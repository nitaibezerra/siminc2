<?php
/**
 * Zend Framework
 *
 * LICENSE
 *
 * This source file is subject to the new BSD license that is bundled
 * with this package in the file LICENSE.txt.
 * It is also available through the world-wide-web at this URL:
 * http://framework.zend.com/license/new-bsd
 * If you did not receive a copy of the license and are unable to
 * obtain it through the world-wide-web, please send an email
 * to license@zend.com so we can send you a copy immediately.
 *
 * @category   Zend
 * @package    Zend_View
 * @subpackage Helper
 * @copyright  Copyright (c) 2005-2012 Zend Technologies USA Inc. (http://www.zend.com)
 * @license    http://framework.zend.com/license/new-bsd     New BSD License
 * @version    $Id: FormSelect.php 24594 2012-01-05 21:27:01Z matthew $
 */


/**
 * Abstract class for extension
 */
require_once 'Zend/View/Helper/FormElement.php';


/**
 * Helper to generate "select" list of options
 *
 * @category   Zend
 * @package    Zend_View
 * @subpackage Helper
 * @author     Marcelo Rodovalho <marcelorodovalho@mec.gov.br>
 * @copyright  Copyright (c) 2005-2012 Zend Technologies USA Inc. (http://www.zend.com)
 * @license    http://framework.zend.com/license/new-bsd     New BSD License
 */
class Simec_View_Helper_Transfer extends Simec_View_Helper_Element
{
    public function transfer($name, $label = null, array $values = array(), array $options = array(), $attribs = array(), $config = array())
    {
        if (substr($name, -2) !== '[]') {
            throw new Exception('O atributo "name" do Transfer, preciso terminar com "[]"');
        }
        $currentName = substr_replace($name, 'Available', -2, 0);

        $html = '<div class="row">
                            <div class="col-lg-5">
                                <div class="form-group">
                                ' . $this->buildSelect($currentName, $values, $attribs, $config, null, true) . '
                                </div>
                            </div>
                            <div class="col-lg-2">
                                <div class="center-block text-center vert-offset-top-2">
                                    <button type="button" id="transfer_add" name="transfer[add]" class="btn btn-success fa fa-play transfer-add"> </button>
                                    <div class="clear-fix"></div>
                                    <button type="button" id="transfer_remove" name="transfer[remove]" class="btn btn-success fa fa-play fa-flip-horizontal transfer-remove vert-offset-top-1"> </button>
                                    <div class="clear-fix"></div>
                                    <button type="button" id="transfer_add_all" name="transfer[add_all]" class="btn btn-success fa fa-forward transfer-add-all"> </button>
                                    <div class="clear-fix"></div>
                                    <button type="button" id="transfer_remove_all" name="transfer[remove_all]" class="btn btn-success fa fa-backward transfer-remove-all"> </button>
                                </div>
                            </div>
                            <div class="col-lg-5">
                                <div class="form-group">
                                    ' . $this->buildSelect($name, $options, $attribs, $config, null, false) . '
                                </div>
                            </div>
                        </div>';
        $html .= "<script>
        $(document).ready(function () {
            $('.transfer-add').on('click', function (event) {
                $('.transfer-assigned').append($('.transfer-available :selected').attr('selected', false));
            });
            $('.transfer-remove').on('click', function (event) {
                $('.transfer-available').append($('.transfer-assigned :selected').attr('selected', false));
            });
            $('.transfer-add-all').on('click', function (event) {
                $('.transfer-assigned').append($('.transfer-available option'));
            });
            $('.transfer-remove-all').on('click', function (event) {
                $('.transfer-available').append($('.transfer-assigned option'));
            });
            var submitTransfer = function (event) {
                event.preventDefault();
                var choicesAssigned = $('.transfer-assigned option'),
                    valAssigned = [];
                if (choicesAssigned.length) {
                    choicesAssigned.each(function () {
                        valAssigned.push($(this).val());
                        $(this).attr('selected', true).prop('selected', true);
                    });
                } else {
                    $('.transfer-assigned').append(
                        '<option selected value=\"\"></option>'
                    );
                }
                $(this).parents('form').submit();
            };
            $('.transfer-add').parents('form').find('[type=submit]').on('click', submitTransfer);
        });</script>";
        return $html;
//        $xhtml = $this->buildTransfer($name, $value, $attribs, $options);
//        return $this->buildField($xhtml, $label, $attribs, $config);

    }

    public function buildSelect($name, $options = null, $attribs = null, $config = array(), $value = null, $booAvailable = true)
    {
        $name1 = $name;
        $info = $this->_getInfo($name, $value, $attribs, $options);
        $help = isset($attribs['help']) ? true : false;
        extract($info); // name, id, value, attribs, options, listsep, disable
        $name = $name1;
        if (null === $id) {
            $id = str_replace(['[', ']'], '', $name);
        }

        // force $value to array so we can compare multiple values to multiple
        // options; also ensure it's a string for comparison purposes.
        $value = array_map('strval', (array)$value);

        // check if element may have multiple values
        $multiple = '';

        if (substr($name, -2) == '[]') {
            // multiple implied by the name
            $multiple = ' multiple="multiple"';
        }

        if (isset($attribs['multiple'])) {
            // Attribute set
            if ($attribs['multiple']) {
                // True attribute; set multiple attribute
                $multiple = ' multiple="multiple"';

                // Make sure name indicates multiple values are allowed
                if (!empty($multiple) && (substr($name, -2) != '[]')) {
                    $name .= '[]';
                }
            } else {
                // False attribute; ensure attribute not set
                $multiple = '';
            }
            unset($attribs['multiple']);
        }

        // now start building the XHTML.
        $disabled = '';
        if (true === $disable) {
            $disabled = ' disabled="disabled"';
        }

        if ($help) {
            $help = "<span class='help-block m-b-none'><i class='fa fa-question-circle' style='color: #1c84c6;'></i> {$attribs['help']}</span>";
        }

        // Build the surrounding select element first.
        $xhtml = '<select '
            . ' class="form-control ' . ($booAvailable ? 'transfer-available' : 'transfer-assigned') . '"'
            . ' size="7"'
            . ' name="' . $this->view->escape($name) . '"'
            . ' id="' . $this->view->escape($id) . '"'
            . $multiple
            . $disabled
            . $this->_htmlAttribs($attribs)
            . ">\n    ";

        // build the list of options
        $list = array();
        $translator = $this->getTranslator();
        foreach ((array)$options as $opt_value => $opt_label) {
            if (is_array($opt_label)) {
                $opt_disable = '';
                if (is_array($disable) && in_array($opt_value, $disable)) {
                    $opt_disable = ' disabled="disabled"';
                }
                if (null !== $translator) {
                    $opt_value = $translator->translate($opt_value);
                }
                $opt_id = ' id="' . $this->view->escape($id) . '-optgroup-'
                    . $this->view->escape($opt_value) . '"';
                $list[] = '<optgroup'
                    . $opt_disable
                    . $opt_id
                    . ' label="' . $this->view->escape($opt_value) . '">';
                foreach ($opt_label as $val => $lab) {
                    $list[] = $this->_build($val, $lab, $value, $disable);
                }
                $list[] = '</optgroup>';
            } else {
                $list[] = $this->_build($opt_value, $opt_label, $value, $disable);
            }
        }

        // add the options to the xhtml and close the select
        $xhtml .= implode("\n    ", $list) . "\n</select>";

        $xhtml .= $help;

        return $xhtml;
    }

    /**
     * Builds the actual <option> tag
     *
     * @param string $value Options Value
     * @param string $label Options Label
     * @param array $selected The option value(s) to mark as 'selected'
     * @param array|bool $disable Whether the select is disabled, or individual options are
     * @return string Option Tag XHTML
     */
    protected function _build($value, $label, $selected, $disable)
    {
        if (is_bool($disable)) {
            $disable = array();
        }
        /*
         * Por favor corriga esta caquinha
         */
        //$this->view->setEscape(function ($dado){ return htmlspecialchars($dado, ENT_COMPAT, 'iso-8859-1'); });
        $opt = '<option'
            . ' value="' . ($value) . '"'
            . ' label="' . ($label) . '"';

        // selected?
        if (in_array((string)$value, $selected)) {
            $opt .= ' selected="selected"';
        }

        // disabled?
        if (in_array($value, $disable)) {
            $opt .= ' disabled="disabled"';
        }

        $opt .= '>' . ($label) . "</option>";

        return $opt;
    }

}
