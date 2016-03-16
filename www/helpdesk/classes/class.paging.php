<?php
/**
 * @category  Class
 * @package   Admin
 * @author    Ian Warner <iwarner@triangle-solutions.com>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version   SVN: $Id: paging.class.php 5 2005-12-13 01:54:06Z nicolas $
 * @since     File available since Release 1.1.1.1
 * \\||
 */

/**
 * The purpose of this class is to provide consistent styling and
 * variables for paginated data. Its role is not to cur_page data,
 * only to cur_page links to paginated pages.
 *
 * Usage:
 *   $pager = new common_Paging($conn, 20, 'admin');
 *   $rs  = &$conn->Execute($pager->getPaginatedQuery("SELECT * FROM table"));
 *   echo $pager->showPaging($_SERVER['PHP_SELF'] . '?' . htmlentities($_SERVER['QUERY_STRING']);
 */
class PHPST_Paging {

    /**
     * The number of the current page.
     *
     * @var int
     */
    private $cur_page = 1;

    /**
     * The number of the next page.
     *
     * @var int
     */
    private $next_page = 1;

    /**
     * The number of the previous page.
     *
     * @var int
     */
    private $prev_page = 1;

    /**
     * The total number of pages/
     *
     * @var int
     */
    private $num_pages = 1;

    /**
     * A unique id used to differentiate among different pagers on the same page.
     *
     * @var string
     */
    private $id = '1';

    /**
     * The number of rows to show per page.
     *
     * @var int
     */
    private $per_page = 10;

    /**
     * A ADODB Connection object to the current DB connection.
     *
     * @var db connection object
     */
    private $conn = null;

    /**
     * Constructor method.
     *
     * @param $conn
     * @param $id
     */
    public function __construct($conn, $per_page, $id = '1') {
        if (!isset($_GET[$id . '_cur_page'])) {
            $_GET[$id . '_cur_page'] = 1;
        }

        $this->setId($id);
        $this->setConn($conn);
        $this->setPer_page($per_page);
        $this->setCur_page($_GET[$id . '_cur_page']);
        $this->setPrev_page($this->getCur_page() - 1);
        $this->setNext_page($this->getCur_page() + 1);
    }

    /**
     * Calculates and sets the number of pages for this pager.
     *
     * @access public
     * @param int $num_rows
     * @return int $num_pages
     */
    public function calcNum_pages($num_rows) {
        if ($num_rows <= $this->getPer_page()) {
            $this->SetNum_pages(1);
        } elseif (($num_rows % $this->getPer_page()) == 0) {
            $this->setNum_pages((int) ($num_rows / $this->getPer_page()));
        } else {
            $this->setNum_pages((int) ($num_rows / $this->getPer_page()) + 1);
        }
    }

    /**
     * Returns a string of links to paginated pages.
     *
     * @param string $page The URL of the paginated page
     * @param int $cur_page The number of rows to cur_page per page
     * @return string The links
     */
    public function showPaging($page)
    {
        $page = explode('&amp;' . $this->getId() . '_cur_page', $page);
        $string = '
        <table class="tbl">
          <tr>';

        if ($this->getPrev_page()) {
            $string.= '<td width="75">
                <a href="' . $page['0'] . '&amp;' . $this->getId() .
                //'_cur_page=' . $this->getPrev_page() . '">&#171;&nbsp;Previous</a>';
                '_cur_page=' . $this->getPrev_page() . '">&#171;&nbsp;' . PHPST_PREVIOUS . '</a>';
        } else {

            //$string .= '<td width="75">&#171;&nbsp;Previous';
            $string .= '<td width="75">&#171;&nbsp;' . PHPST_PREVIOUS;
        }

        $string .= '</td>
            <td>';

        if (!is_int($this->getCur_page())) {
            $start = 1;
            $end   = 10;
        } else {
            $end = $this->getCur_page() + 9;

            if ($this->getCur_page() > 20) {
                $start = $this->getCur_page() - 19;

                $string .= '<a href="' . $page['0'] . '&amp;' . $this->getId() .
                '_cur_page=1">1</a>..';
            } else {
                $start = 1;
            }
        }

        if ($end > $this->getNum_pages()) {
            $end = $this->getNum_pages();
        }

        for ($i = $start; $i <= $end; $i++) {
            if ($i != $this->getCur_page()) {
                $string .= ' <a href="' . $page['0'] . '&amp;' . $this->getId() .
                '_cur_page=' . $i . '">' . $i . '</a> ';
            } else {
                $string .= '[<b>' . $i . '</b>]';
            }
        }

        if ($end < $this->getNum_pages()) {
            $string .= '..<a href="' . $page['0'] . '&amp;' . $this->getId() .
            '_cur_page=' . $this->getNum_pages() . '">' .
            $this->getNum_pages() . '</a>';
        }
        $string .= '</td>';

        if ($this->getCur_page() != $this->getNum_pages()) {
            $string .= '<td width="75"><a href="' . $page['0'] . '&amp;' .
            $this->getId() . '_cur_page=' .
            //$this->getNext_page() . '">Next&nbsp;&#187;</a>';
            $this->getNext_page() . '">' . PHPST_NEXT . '&nbsp;&#187;</a>';

        } else {
            //$string .= '<td width="75">Next&nbsp;&#187;';
            $string .= '<td width="75">' . PHPST_NEXT . '&nbsp;&#187;';
        }
        $string .= '</td>
          </tr>
        </table>';
        return $string;
    }

    /**
     * Takes a SELECT query string and applies a pagination limit upon it,
     * then returns the modified query.
     *
     * @access public
     *
     * @param string $sql The SQL string being edited
     * @return string
     */
    public function getPaginatedQuery($sql) {
        $conn = $this->getConn();

        $rs  = &$conn->Execute($sql);
        if (!$rs) {
            echo '<p>Error ' . $conn->ErrorMsg() . '</p>';
        }

        $page_start = ($this->getPer_page() * $this->getCur_page()) - $this->getPer_page();
        $num_rows = $rs->RecordCount();

        if ($page_start < '0') {
            $page_start = '0';
        }

        $this->calcNum_pages($num_rows);

        $sql = $sql . " LIMIT " . $page_start . ", " . $this->getPer_page();
        return $sql;
    }

    // GETTERS
    /**
     * Returns this pager's id.
     *
     * @return string
     */
    public function getId() {
        return $this->id;
    }

    /**
     * Returns this pager's db connection object.
     *
     * @return adodbdb connection object
     */
    public function getConn() {
        return $this->conn;
    }

    /**
     * Returns this pager's next page number.
     *
     * @return string
     */
    public function getNext_page() {
        return $this->next_page;
    }

    /**
     * Returns this pager's previous page number.
     *
     * @return string
     */
    public function getPrev_page() {
        return $this->prev_page;
    }

    /**
     * Returns this pager's current page number.
     *
     * @return string
     */
    public function getCur_page() {
        return $this->cur_page;
    }

    /**
     * Returns this pager's number of rows per page.
     *
     * @return string
     */
    public function getPer_page() {
        return $this->per_page;
    }

    /**
     * Returns this pager's number of pages.
     *
     * @return string
     */
    public function getNum_pages() {
        return $this->num_pages;
    }


    // SETTERS
    /**
     * Sets this pager's id.
     *
     * @param $value string
     */
    public function setId($value) {
        $this->id = $value;
    }

    /**
     * Sets this pager's adodb Connection object.
     *
     * @param $value adodb Connection object
     */
    public function setConn($value) {
        $this->conn = $value;
    }

    /**
     * Sets this pager's next page number.
     *
     * @param $value string
     */
    public function setNext_page($value) {
        $this->next_page = $value;
    }

    /**
     * Sets this pager's previous page number.
     *
     * @param $value string
     */
    public function setPrev_page($value) {
        $this->prev_page = $value;
    }

    /**
     * Sets this pager's current page number.
     *
     * @param $value string
     */
    public function setCur_page($value) {
        $this->cur_page = $value;
    }

    /**
     * Sets this pager's number of rows per page.
     *
     * @param $value string
     */
    public function setPer_page($value) {
        $this->per_page = $value;
    }

    /**
     * Sets this pager's number of pages.
     *
     * @param $value string
     */
    public function setNum_pages($value) {
        $this->num_pages = $value;
    }

}
?>
