<?php
/**
 * Abstract class for extension
 */
require_once 'Zend/View/Helper/FormElement.php';


class Simec_View_Helper_FormElement extends Zend_View_Helper_FormElement
{

    protected function buildField($xhtml, $label, $config)
    {
        $icon = !empty($config['icon']) ? '<span class="input-group-addon"><span class="glyphicon glyphicon-' . $config['icon'] . '"></span></span></span>' : null;
        $help = !empty($config['help']) ? '<span class="input-group-addon help-tooltip" data-toggle="tooltip" data-placement="left" title="' . $config['help'] . '"><span class="glyphicon glyphicon-question-sign"></span></span>' : null;

        if ($icon || $help) {
            $xhtml = '
                <div class="input-group">
                    ' . $icon . '
                    ' . $xhtml . '
                    ' . $help . '
                </div>
            ';
        }


        if ($label) {
            $required = is_array($config) && in_array('required', $config) ? '<span class="campo-obrigatorio" title="Campo obrigatório">*</span>' : '';

            return '
                <div class="form-group">
                    <label for="' . $config['label-for'] . '" class="col-lg-2 control-label">' . $label . ': ' . $required . '</label>
                    <div class="col-lg-10">
                        '. $xhtml .'
                    </div>
                </div>
            ';
        }
        return $xhtml;
    }
}
