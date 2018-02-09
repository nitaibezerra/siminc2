<?php
/**
 * Abstract class for extension
 */
require_once 'Zend/View/Helper/FormElement.php';


class Simec_View_Helper_Options extends Simec_View_Helper_Element
{
    public function options($name, $label = null, $value = null, $options = null, $attribs = null, $config = array())
    {
        $value = (array)$value;

        // Array veio do método carregar
        if (!empty($value) && is_array(current($value)) && count(current($value)) == 1) {
            $valores = array();
            foreach ($value as $valor) {
                $valores[] = current($valor);
            }
            $value = $valores;
        }

        foreach ($attribs as $chave => $attrib) {
            if (is_numeric($chave)) {
                $attribs[$attrib] = $attrib;
                unset($attribs[$chave]);
            }
        }

        $type = isset($attribs['type']) ? $attribs['type'] : 'radio';
        $style = isset($config['style']) ? $config['style'] : 'button';
        $help = isset($attribs['help']) ? true : false;
        $disabled = $attribs['disabled'] == 'disabled' ? 'disabled' : '';

        switch ($style) {
            case ('inline'):
            case ('lista'):
                $classeTipo = $type . '-' . $style;
                $xhtml = '';
                foreach ($options as $valor => $descricao) {
                    $id = isset($attribs['id']) ? $attribs['id'] : $name . '-' . $valor;
                    $attribs['checked'] = in_array($valor, $value) ? 'checked' : null;
                    $xhtml.= '<div class="' . $type . ' ' . $classeTipo . ' ' . $disabled . '">';
                    $xhtml.= $this->montarInput($name, $valor, null, $attribs, $config);
                    $xhtml.= '<label for="' . $id . '">' . $descricao . '</label>';
                    $xhtml.= '</div>';
                }
                break;
            case ('icheckbox_square-green'):
                $classeTipo = $type . '-' . $style;
                $xhtml = '';
                foreach ($options as $valor => $descricao) {
                    $id = isset($attribs['id']) ? $attribs['id'] : $name . '-' . $valor;
                    $attribs['checked'] = in_array($valor, $value) ? 'checked' : null;
                    $xhtml.= '<div class="' . $type . ' ' . $classeTipo . ' ' . $disabled . '">';
                    $xhtml.= $this->montarInput($name, $valor, null, $attribs, $config);
                    $xhtml.= '<label for="' . $id . '">' . $descricao . '</label>';
                    $xhtml.= '</div>';
                }
                break;
            default:
                $xhtml = '<div class="btn-group" data-toggle="buttons">';
                $id = $attribs['id'];
                foreach ($options as $valor => $descricao) {
                    $marcado = in_array($valor, $value) ? ' active' : '';
                    $attribs['id'] = isset($attribs['id']) ? $id. '_' . $valor : $name . '_' . $valor;
                    $attribs['checked'] = in_array($valor, $value) ? 'checked' : null;
                    $xhtml .= ' <label class="btn btn-primary btn-campo'. $marcado . ' ' . $disabled . ' ' . $attribs['id'] .'">
                                    ' . $this->montarInput($name, $valor, $descricao, $attribs, $config) . '
                                </label>';
                }

                $xhtml .= '</div>';
            break;
        }

        if ($help) {
            $xhtml.= "<span class='help-block m-b-none'><i class='fa fa-question-circle' style='color: #1c84c6;'></i> {$attribs['help']}</span>";
        }

        // seta o required para ser usado no build.
        $config['required'] = empty($attribs['required'])? '' : $attribs['required'];

        return $this->_build($xhtml, $label, $config);
    }

    protected function montarInput($name, $valor, $descricao, $attribs = null, $config = null)
    {
        $attribs = (array) $attribs;

        foreach ($attribs as $chave => $attrib) {
            if (is_numeric($chave)) {
                $attribs[$attrib] = $attrib;
                unset($attribs[$chave]);
            }
        }

        $id = isset($attribs['id']) ? $attribs['id'] : $name . '-' . $valor;
        $type = isset($attribs['type']) ? $attribs['type'] : 'radio';
        $checked = isset($attribs['checked']) && $attribs['checked'] ? 'checked="checked"' : '';
        $class = isset($attribs['class']) ? 'class="form-control ' .  $attribs['class'] . '"' : '';
        $required = is_array($attribs) && in_array('required', $attribs) ? 'required="required"' : '';

        unset($attribs['id'], $attribs['type'], $attribs['class'], $attribs['help'], $attribs['checked']);

        $xhtml = '<input'
               . ' name="' . $this->view->escape($name) . '"'
               . ' id="' . $this->view->escape($id) . '"'
               . ' type="' . $type . '"'
               . ' value="' . $valor . '"'
               . ' ' . $checked . ' '
               . ' ' . $class . ' '
               . ' ' . $required . ' '
               . $this->_htmlAttribs($attribs)
               . " />" . $descricao;

        return $xhtml;
    }

    protected function _build($xhtml, $label, $config)
    {
        $icon = !empty($config['icon']) ? '<span class="input-group-addon"><span class="glyphicon glyphicon-' . $config['icon'] . '"></span></span></span>' : null;
        $help = !empty($config['help']) ? '<span class="input-group-addon help-tooltip" data-toggle="tooltip" data-placement="left" title="' . $config['help'] . '"><span class="glyphicon glyphicon-question-sign"></span></span>' : null;
        $labelSize = !empty($config['label-size']) ? $config['label-size'] : 3;
        $inputSize = !empty($config['input-size']) ? $config['input-size'] : 9;

        if (isset($config['visible']) && $config['visible'] == false) {
        	$classGroup.= ' hidden ';
       	}

        if (isset($config['formTipo']) && $config['formTipo'] == Simec_View_Helper::K_FORM_TIPO_VERTICAL) {
            $classLabel = '';
            $classInput = ' ';
        } elseif ($labelSize || $inputSize) {
            $classLabel = "col-sm-{$labelSize} col-md-{$labelSize} col-lg-{$labelSize} control-label";
            $classInput = "col-sm-{$inputSize} col-md-{$inputSize} col-lg-{$inputSize} ";
        } else {
            $classLabel = 'col-sm-3 col-md-3 col-lg-3 control-label';
            $classInput = 'col-sm-9 col-md-9 col-lg-9 ';
        }

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
            $required = is_array($config) && !empty($config['required']) ? '<span class="campo-obrigatorio" title="Campo obrigatório">*</span>' : '';
            $xhtml =  '
                <div class="form-group '.$classGroup. '">
                    <label for="intcnpj" class="'. $classLabel .'  control-label">' . $label . ': ' . $required . '</label>
                    <div class="' . $classInput . '">
                        '. $xhtml .'
                    </div>
                    <div style="clear:both"></div>
                </div>
            ';
        }

        return $xhtml;
    }
}
