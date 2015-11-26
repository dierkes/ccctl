/*
 * red_black_tree.h
 *
 *  Created on: Oct 11, 2015
 *      Author: dierkes
 *
 * This code is an adaption of the red-black tree implementation of EASTL. It is therefore under the
 * additional following copyright license:
 *
 * Copyright (C) 2005,2009-2010 Electronic Arts, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Electronic Arts, Inc. ("EA") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ELECTRONIC ARTS AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ELECTRONIC ARTS OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef CCC_RED_BLACK_TREE_H_
#define CCC_RED_BLACK_TREE_H_

#include <ciso646>
#include <cstddef>
#include <cstring>
#include <stdexcept>

#include <ccc/compat.h>
#include <ccc/memory.h>
#include <ccc/type_traits.h>
#include <ccc/alignment.h>

#include <ccc/storage.h>

namespace ccc
{

/**
 * @brief Consistent, static-capacity array.
 *
 */
template<typename T, typename SizeType, SizeType Capacity, std::size_t Alignment = 8,
        bool UseRawMemOps = false, bool StaticStorage = true>
struct PodRedBlackTree
{
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
    typedef SizeType size_type;
    typedef std::ptrdiff_t difference_type;

    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    struct NodeColor
    {
        enum EnumType
        {
            Red, Black
        };

        typedef char IntType;
    };

    struct TreeSide
    {
        enum EnumType
        {
            Left, Right
        };

        typedef char IntType;
    };

    struct TreeNode
    {
        SizeType Left;
        SizeType Right;
        SizeType Parent;
        NodeColor::IntType Color;
    };

    struct DataNode
    {
        TreeNode Tree;
        T Data;
    };

    // support zero-sized arrays (see std::array in gcc)
    typedef typename StorageType<DataNode, SizeType, Capacity ? Capacity : 1, Alignment,
            StaticStorage, true>::type storage_type;
    storage_type m_Storage;

    PaddedValue<TreeNode, Alignment> m_Anchor;

    typedef SizeType index_type;

    index_type& Parent(index_type Node)
    {
        if (Node != 0)
        {
            return m_Storage[Node - 1].Tree.Parent;
        }
        else
        {
            return m_Anchor.Parent;
        }
    }

    index_type& Left(index_type Node)
    {
        if (Node != 0)
        {
            return m_Storage[Node - 1].Tree.Left;
        }
        else
        {
            return m_Anchor.Left;
        }
    }

    index_type& Right(index_type Node)
    {
        if (Node != 0)
        {
            return m_Storage[Node - 1].Tree.Right;
        }
        else
        {
            return m_Anchor.Right;
        }
    }

    NodeColor::EnumType& Color(index_type Node)
    {
        if (Node != 0)
        {
            return static_cast<NodeColor::EnumType&>(m_Storage[Node - 1].Tree.Color);
        }
        else
        {
            return static_cast<NodeColor::EnumType&>(m_Anchor.Color);
        }
    }

    T& Value(index_type Node)
    {
        if (Node != 0)
        {
            return m_Storage[Node - 1].Data;
        }
        else
        {
            throw std::bad_alloc("Tried to access data of anchor node.");
        }
    }

    bool HasParent(index_type Node)
    {
        if (Node != 0)
        {
            return 0 != m_Storage[Node - 1].Tree.Parent;
        }
        else
        {
            return 0 != m_Anchor.Parent;
        }
    }

    bool HasLeft(index_type Node)
    {
        if (Node != 0)
        {
            return 0 != m_Storage[Node - 1].Tree.Left;
        }
        else
        {
            return 0 != m_Anchor.Left;
        }
    }

    bool HasRight(index_type Node)
    {
        if (Node != 0)
        {
            return 0 != m_Storage[Node - 1].Tree.Right;
        }
        else
        {
            return 0 != m_Anchor.Right;
        }
    }

    /// RBTreeIncrement
    /// Returns the next item in a sorted red-black tree.
    ///
    index_type Increment(index_type Node)
    {
        if (0 != Right(Node))
        {

            Node = Right(Node);

            while (HasLeft(Node))
            {
                Node = Left(Node);
            }
        }
        else
        {
            index_type Temp = Parent(Node);

            while (Node == Right(Temp))
            {
                Node = Temp;
                Temp = Parent(Node);
            }

            if (Right(Node) != Temp)
            {
                Node = Temp;
            }
        }

        return Node;
    }

    /// RBTreeIncrement
    /// Returns the previous item in a sorted red-black tree.
    ///
    index_type Decrement(index_type Node)
    {
        if ((Parent(Parent(Node)) == Node) && (Color(Node) == NodeColor::Red))
        {
            return Right(Node);
        }
        else if (HasLeft(Node))
        {
            Node = Left(Node);

            while (HasRight(Node))
            {
                Node = Right(Node);
            }

            return Node;
        }
        else
        {
            index_type Temp = Parent(Node);

            while (Node == Left(Temp))
            {
                Node = Temp;
                Temp = Parent(Node);
            }

            return Temp;
        }
    }

    /// RBTreeGetBlackCount
    /// Counts the number of black nodes in an red-black tree, from pNode down to the given bottom node.
    /// We don't count red nodes because red-black trees don't really care about
    /// red node counts; it is black node counts that are significant in the
    /// maintenance of a balanced tree.
    ///
    index_type BlackCount(index_type Top, index_type Bottom)
    {
        size_type nCount = 0;

        for (; 0 != Bottom; Bottom = Parent(Bottom))
        {
            if (Color(Bottom) == NodeColor::Black)
            {
                ++nCount;
            }

            if (Bottom == Top)
            {
                break;
            }
        }

        return nCount;
    }

    /// RBTreeRotateLeft
    /// Does a left rotation about the given node.
    /// If you want to understand tree rotation, any book on algorithms will
    /// discussion the topic in good detail.
    index_type RotateLeft(index_type Node, index_type Root)
    {
        index_type Temp = Right(Node);

        Right(Node) = Left(Temp);

        if (HasLeft(Temp))
        {
            Parent(Left(Temp)) = Node;
        }
        Parent(Temp) = Parent(Node);

        if (Node == Root)
        {
            Root = Temp;
        }
        else if (Node == Left(Parent(Node)))
        {
            Left(Parent(Node)) = Temp;
        }
        else
        {
            Right(Parent(Node)) = Temp;
        }

        Left(Temp) = Node;
        Parent(Node) = Temp;

        return Root;
    }

    /// RBTreeRotateRight
    /// Does a right rotation about the given node.
    /// If you want to understand tree rotation, any book on algorithms will
    /// discussion the topic in good detail.
    index_type RotateRight(index_type Node, index_type Root)
    {
        index_type Temp = Left(Node);

        Left(Node) = Right(Temp);

        if (HasRight(Temp))
        {
            Parent(Right(Temp)) = Node;
        }
        Parent(Temp) = Parent(Node);

        if (Node == Root)
        {
            Root = Temp;
        }
        else if (Node == Right(Parent(Node)))
        {
            Right(Parent(Node)) = Temp;
        }
        else
        {
            Left(Parent(Node)) = Temp;
        }

        Right(Temp) = Node;
        Parent(Node) = Temp;

        return Root;
    }

    /// RBTreeInsert
    /// Insert a node into the tree and rebalance the tree as a result of the
    /// disturbance the node introduced.
    ///
    void InsertNode(index_type Node, index_type NodeParent, index_type NodeAnchor,
            TreeSide::EnumType InsertionSide)
    {
        index_type& RootRef = Parent(NodeAnchor);

        // Initialize fields in new node to insert.
        Parent(Node) = NodeParent;
        Right(Node) = 0;
        Left(Node) = 0;
        Color(Node) = NodeColor::Red;

        // Insert the node.
        if (TreeSide::Left == InsertionSide)
        {
            Left(NodeParent) = Node; // Also makes (leftmost = pNode) when (pNodeParent == pNodeAnchor)

            if (NodeParent == NodeAnchor)
            {
                Parent(NodeAnchor) = Node;
                Right(NodeAnchor) = Node;
            }
            else if (NodeParent == Left(NodeAnchor))
            {
                Left(NodeAnchor) = Node; // Maintain leftmost pointing to min node
            }
        }
        else
        {
            Right(NodeParent) = Node;

            if (NodeParent == Right(NodeAnchor))
            {
                Right(NodeAnchor) = Node; // Maintain rightmost pointing to max node
            }
        }

        // Rebalance the tree.
        while ((Node != RootRef) && (NodeColor::Red == Color(Parent(Node))))
        {
            const index_type NodeParentParent = Parent(Parent(Node));

            if (Parent(Node) == Left(NodeParentParent))
            {
                const index_type Temp = Right(NodeParentParent);

                if ((0 != Temp) && (NodeColor::Red == Color(Temp)))
                {
                    Color(Parent(Node)) = NodeColor::Black;
                    Color(Temp) = NodeColor::Black;
                    Color(NodeParentParent) = NodeColor::Red;
                    Node = NodeParentParent;
                }
                else
                {
                    if (Node == Right(Parent(Node)))
                    {
                        Node = Parent(Node);
                        RootRef = RotateLeft(Node, RootRef);
                    }

                    Color(Parent(Node)) = NodeColor::Black;
                    Color(NodeParentParent) = NodeColor::Red;
                    RootRef = RotateRight(NodeParentParent, RootRef);
                }
            }
            else
            {
                const index_type Temp = Left(NodeParentParent);

                if ((0 != Temp) && (NodeColor::Red == Color(Temp)))
                {
                    Color(Parent(Node)) = NodeColor::Black;
                    Color(Temp) = NodeColor::Black;
                    Color(NodeParentParent) = NodeColor::Red;
                    Node = NodeParentParent;
                }
                else
                {
                    if (Node == Left(Parent(Node)))
                    {
                        Node = Parent(Node);
                        RootRef = RotateRight(Node, RootRef);
                    }

                    Color(Parent(Node)) = NodeColor::Black;
                    Color(NodeParentParent) = NodeColor::Red;
                    RootRef = RotateLeft(NodeParentParent, RootRef);
                }
            }
        }

        Color(RootRef) = NodeColor::Black;

    } // RBTreeInsert

    /// RBTreeErase
    /// Erase a node from the tree.
    ///
    void EraseNode(index_type Node, index_type NodeAnchor)
    {
        index_type& RootRef = Parent(NodeAnchor);
        index_type& LeftmostRef = Left(NodeAnchor);
        index_type& RightmostRef = Right(NodeAnchor);
        index_type NodeSuccessor = Node;
        index_type NodeChild = 0;
        index_type NodeChildParent = 0;

        if (not HasLeft(NodeSuccessor)) // pNode has at most one non-NULL child.
        {
            NodeChild = Right(NodeSuccessor); // pNodeChild might be null.
        }
        else if (not HasRight(NodeSuccessor)) // pNode has exactly one non-NULL child.
        {
            NodeChild = Left(NodeSuccessor); // pNodeChild is not null.
        }
        else
        {
            // pNode has two non-null children. Set pNodeSuccessor to pNode's successor. pNodeChild might be NULL.
            NodeSuccessor = Right(NodeSuccessor);

            while (HasLeft(NodeSuccessor))
            {
                NodeSuccessor = Left(NodeSuccessor);
            }

            NodeChild = Right(NodeSuccessor);
        }

        // Here we remove pNode from the tree and fix up the node pointers appropriately around it.
        if (NodeSuccessor == Node) // If pNode was a leaf node (had both NULL children)...
        {
            NodeChildParent = Parent(NodeSuccessor); // Assign pNodeReplacement's parent.

            if (0 != NodeChild)
            {
                Parent(NodeChild) = Parent(NodeSuccessor);
            }

            if (Node == RootRef) // If the node being deleted is the root node...
            {
                RootRef = NodeChild; // Set the new root node to be the pNodeReplacement.
            }
            else
            {
                if (Node == Left(Parent(Node))) // If pNode is a left node...
                {
                    Left(Parent(Node)) = NodeChild; // Make pNode's replacement node be on the same side.
                }
                else
                {
                    Right(Parent(Node)) = NodeChild;
                }
                // Now pNode is disconnected from the bottom of the tree (recall that in this pathway pNode was determined to be a leaf).
            }

            if (Node == LeftmostRef) // If pNode is the tree begin() node...
            {
                // Because pNode is the tree begin(), pNode->mpNodeLeft must be NULL.
                // Here we assign the new begin() (first node).
                if (HasRight(Node))
                {
                    LeftmostRef = MinChild(NodeChild);
                }
                else
                {
                    LeftmostRef = Parent(Node); // This  makes (pNodeLeftmostRef == end()) if (pNode == root node)
                }
            }

            if (Node == RightmostRef) // If pNode is the tree last (rbegin()) node...
            {
                // Because pNode is the tree rbegin(), pNode->mpNodeRight must be NULL.
                // Here we assign the new rbegin() (last node)
                if (HasLeft(Node))
                {
                    RightmostRef = MaxChild(NodeChild);
                }
                else // pNodeChild == pNode->mpNodeLeft
                {
                    RightmostRef = Parent(Node); // makes pNodeRightmostRef == &mAnchor if pNode == pNodeRootRef
                }
            }
        }
        else // else (pNodeSuccessor != pNode)
        {
            // Relink pNodeSuccessor in place of pNode. pNodeSuccessor is pNode's successor.
            // We specifically set pNodeSuccessor to be on the right child side of pNode, so fix up the left child side.
            Parent(Left(Node)) = NodeSuccessor;
            Left(NodeSuccessor) = Left(Node);

            if (NodeSuccessor == Right(Node)) // If pNode's successor was at the bottom of the tree... (yes that's effectively what this statement means)
            {
                NodeChildParent = NodeSuccessor; // Assign pNodeReplacement's parent.
            }
            else
            {
                NodeChildParent = Parent(NodeSuccessor);

                if (0 != NodeChild)
                {
                    Parent(NodeChild) = NodeChildParent;
                }

                Left(NodeChildParent) = NodeChild;

                Right(NodeSuccessor) = Right(Node);
                Parent(Right(Node)) = NodeSuccessor;
            }

            if (Node == RootRef)
            {
                RootRef = NodeSuccessor;
            }
            else if (Node == Left(Parent(Node)))
            {
                Left(Parent(Node)) = NodeSuccessor;
            }
            else
            {
                Right(Parent(Node)) = NodeSuccessor;
            }

            // Now pNode is disconnected from the tree.

            Parent(NodeSuccessor) = Parent(Node);
            // swap:
            {
                NodeColor::EnumType TmpColor;
                TmpColor = Color(NodeSuccessor);
                Color(NodeSuccessor) = Color(Node);
                Color(Node) = TmpColor;
            }
        }

        // Here we do tree balancing as per the conventional red-black tree algorithm.
        if (NodeColor::Black == Color(Node))
        {
            while ((NodeChild != RootRef)
                    && ((0 == NodeChild) || (NodeColor::Black == Color(NodeChild))))
            {
                if (NodeChild == Left(NodeChildParent))
                {
                    index_type Temp = Right(NodeChildParent);

                    if (NodeColor::Red == Color(Temp))
                    {
                        Color(Temp) = NodeColor::Black;
                        Color(NodeChildParent) = NodeColor::Red;
                        RootRef = RotateLeft(NodeChildParent, RootRef);
                        Temp = Right(NodeChildParent);
                    }

                    if (((not HasLeft(Temp)) || (NodeColor::Black == Color(Left(Temp))))
                            && ((not HasRight(Temp)) || (NodeColor::Black == Color(Right(Temp)))))
                    {
                        Color(Temp) = NodeColor::Red;
                        NodeChild = NodeChildParent;
                        NodeChildParent = Parent(NodeChildParent);
                    }
                    else
                    {
                        if ((not HasRight(Temp)) || (NodeColor::Black == Color(Right(Temp))))
                        {
                            Color(Left(Temp)) = NodeColor::Black;
                            Color(Temp) = NodeColor::Red;
                            RootRef = RotateRight(Temp, RootRef);
                            Temp = Right(NodeChildParent);
                        }

                        Color(Temp) = Color(NodeChildParent);
                        Color(NodeChildParent) = NodeColor::Black;

                        if (HasRight(Temp))
                        {
                            Color(Right(Temp)) = NodeColor::Black;
                        }

                        RootRef = RotateLeft(NodeChildParent, RootRef);
                        break;
                    }
                }
                else
                {
                    // The following is the same as above, with mpNodeRight <-> mpNodeLeft.
                    index_type Temp = Left(NodeChildParent);

                    if (NodeColor::Red == Color(Temp))
                    {
                        Color(Temp) = NodeColor::Black;
                        Color(NodeChildParent) = NodeColor::Red;

                        RootRef = RotateRight(NodeChildParent, RootRef);
                        Temp = Left(NodeChildParent);
                    }

                    if (((not HasRight(Temp)) || (NodeColor::Black == Color(Right(Temp))))
                            && ((not HasLeft(Temp)) || (NodeColor::Black == Color(Left(Temp)))))
                    {
                        Color(Temp) = NodeColor::Red;
                        NodeChild = NodeChildParent;
                        NodeChildParent = Parent(NodeChildParent);
                    }
                    else
                    {
                        if ((not HasLeft(Temp)) || (NodeColor::Black == Color(Left(Temp))))
                        {
                            Color(Right(Temp)) = NodeColor::Black;
                            Color(Temp) = NodeColor::Red;

                            RootRef = RotateLeft(Temp, RootRef);
                            Temp = Left(NodeChildParent);
                        }

                        Color(Temp) = Color(NodeChildParent);
                        Color(NodeChildParent) = NodeColor::Black;

                        if (HasLeft(Temp))
                        {
                            Color(Left(Temp)) = NodeColor::Black;
                        }

                        RootRef = RotateRight(NodeChildParent, RootRef);
                        break;
                    }
                }
            }

            if (0 != NodeChild)
            {
                Color(NodeChild) = NodeColor::Black;
            }
        }

    } // RBTreeErase

    inline index_type MinChild(index_type Node)
    {
        while (HasLeft(Node))
        {
            Node = Left(Node);
        }
        return Node;
    }

    inline index_type MaxChild(index_type Node)
    {
        while (HasRight(Node))
        {
            Node = Right(Node);
        }
        return Node;
    }

    index_type PrivateAllocateNode();
    void PrivateDeallocateNode(index_type Node);

    index_type PrivateCreateNodeFromKey(const key_type& key);
    index_type PrivateCreateNode(const value_type& value);
    index_type PrivateCreateNode(const_index NodeSource, index_type NodeParent);

    index_type PrivateCopySubtree(const_index NodeSource, index_type NodeDest);
    void PrivateEraseSubtree(index_type Node);

    // Intentionally return a pair and not an iterator for DoInsertValue(..., true_type)
    // This is because the C++ standard for map and set is to return a pair and not just an iterator.
    std::pair<iterator, bool> PrivateInsertValue(const value_type& value, true_type); // true_type means keys are unique.
    iterator PrivateInsertValue(const value_type& value, false_type); // false_type means keys are not unique.

    std::pair<iterator, bool> PrivateInsertKey(const key_type& key, true_type);
    iterator DoInsertKey(const key_type& key, false_type);
};

}

#endif /* CCC_RED_BLACK_TREE_H_ */
