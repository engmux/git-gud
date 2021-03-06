#ifndef GIT_GUD_HPP
#define GIT_GUD_HPP

#include <string>
#include <memory>
#include <vector>
#include <map>

namespace git_gud
{

    /**
     * Commits make the nodes of the GitTree. Each may have several
     * parents and several children. Each MUST have a unique ID.
     *
     * @author Michael Bianconi
     */
    class Commit
    {
        private:

            int commitID;
            int branchID;

            std::vector<std::shared_ptr<Commit> > parents;
            std::vector<std::shared_ptr<Commit> > children;

        public:

            /**
             * Creates a new Commit on the specified branch and with
             * a unique ID.
             *
             * @param branchID The ID of the branch.
             */
            Commit(int branchID); // autogenerated ID

            /**
             * Creates a new Commit on the specified branch.
             *
             * @param branchID The ID of the branch.
             * @param commitID The ID of the commit.
             */
            Commit(int branchID, int commitID); // pregenerated ID

            int getID() const;
            int getBranch() const;
            int getNumParents() const;
            int getNumChildren() const;
            bool isMergeCommit() const;
            bool isNewBranch() const;

            std::vector<std::shared_ptr<Commit> >& getParents();
            std::vector<std::shared_ptr<Commit> >& getChildren();

            /**
             * Adds the given commit as a parent. Does NOT update the
             * parent commit to reflect the change.
             *
             * @param parent Parent commit to add.
             * @throws std::invalid_argument if adding itself as a parent.
             */
            void addParent(std::shared_ptr<Commit> parent);

            /**
             * Adds the given commit as a child. Does NOT update the
             * child commit to reflect the change.
             *
             * @param child Child commit to add.
             * @throws std::invalid_argument if adding itself as a child.
             */
            void addChild(std::shared_ptr<Commit> child);

            /**
             * Removes the parent commit from this object.
             *
             * @param id ID of the parent commit.
             * @throws std::invalid_argument if parent does not exist.
             */
            void removeParent(int id);

            /**
             * Removes the child commit from this object.
             *
             * @param id ID of the child commit.
             * @throws std::invalid_argument if child does not exist.
             */
            void removeChild(int id);

            void print();
    };

    class GitTree
    {
        private:

            int nextCommitID = 0;
            int nextBranchID = 0;

            std::shared_ptr<Commit> head;
            int currentBranch;
            std::vector<std::shared_ptr<Commit> > commits;
            std::vector<std::shared_ptr<Commit> > branch_heads;
            int numBranches;

        public:

            GitTree();

            std::shared_ptr<Commit> getHead() const;
            int getCurrentBranch() const;
            bool isHead(int commitID) const;
            std::shared_ptr<Commit> getCommit(int ID) const;
            std::shared_ptr<Commit> getLatest() const;
            std::shared_ptr<Commit> getLatest(int branchID) const;

            /** Guaranteed to have non-repeating IDs. */
            std::vector<int> getAllBranchIDs() const;

            /** If it has the same ID twice, something is very wrong. */
            std::vector<int> getAllCommitIDs() const;

            bool isValidCommitID(int id) const;
            bool isValidBranchID(int id) const;

            std::vector<std::shared_ptr<Commit> >& getAllCommits();
            int getNumBranches() const;
            int getNumCommits() const;

            /**
             * Generates a new Commit as a child of the head.
             * Checks out the new Commit.
             *
             * @return Returns the generated Commit.
             * @throws std::invalid_argument if HEAD already has a child.
             */
            std::shared_ptr<Commit> addCommit();

            /**
             * Generates a new Commit as a child of the specified parent,
             * on the same branch. Checks out the new Commit.
`            *
             * @param parentID ID of the commit to add to.
             * @throws std::invalid_argument if parent already has a child,
             *         or does not exist.
             */
            std::shared_ptr<Commit> addCommit(int parentID);

            /**
             * Sets the head of the tree to the latest commit on the branch.
             *
             * @param branchID ID of the branch to checkout.
             * @throws std::invalid_argument if branchID does not exist.
             */
            void checkout(int branchID);

      /**
       * Creates a new branch but doesn't check it out.
       */
      int branch();

            /**
             * Sets the head of the tree to a specific commit.
             *
             * @param Commit to set.
             */
            void checkoutCommit(int commit);

            /**
             * Creates a new commit, appended to the head and with
             * the given branch's latest commit.
             *
             * @param branchID ID of the branch to merge into the head.
             * @return Returns the new Commit.
             * @throws std::invalid_argument if otherID doesn't exist.
             */
            std::shared_ptr<Commit> merge(int branchID);

            /**
             * Creates a new commit, appended to the parentID and
             * merged with the other.
             *
             * @param parentID ID of the parent to merge onto.
             * @param otherID ID of the brange to merge from.
             * @return Return's the newly created commit.
             * @throws std::invalid_argument if either commit doesn't exist.
             */
            std::shared_ptr<Commit> merge(int parentID, int otherID);

            /**
             * Removes the last Commit from the tree. If there is
             * only one node, do nothing.
             */
            void undo();

            /**
             * Reset everything.
             */
            void reset();

            void print() const;

            static int generateBranchID();
            static int decrementBranchID();
    };
}

#endif
