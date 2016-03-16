<?php
/**
 * Abstract class for extension
 */
require_once 'Zend/View/Helper/FormElement.php';


class Simec_View_Helper_Options extends Simec_View_Helper_Element
{
    public function options($name, $label = null, $value = null, $options = null, $attribs = null, $config = array())
    {
        $id = isset($attribs['id']) ? $attribs['id'] : $name;
        $type = isset($attribs['type']) ? $attribs['type'] : 'radio';
        $config['label-for'] = isset($config['label-for']) ? $config['label-for'] : $id;
        $style = isset($config['style']) ? $config['style'] : 'button';
        $help = isset($attribs['help']) ? true : false;
        
        switch ($style) {
            case ('inline'):
                $xhtml = '<div>';
                foreach ($options as $valor => $descricao) {
                    $xhtml .= ' <label class="' . $type . '-inline">
                                    ' . $this->montarInput($name, $valor, $descricao, $attribs, $config) . '
                                </label>';
                }
                $xhtml .= '</div>';
                break;
            case ('lista'):
                $xhtml = '<div>';
                foreach ($options as $valor => $descricao) {
                    $xhtml .= ' <div class="' . $type . '">
                                    <label>
                                        ' . $this->montarInput($name, $valor, $descricao, $attribs, $config) . '
                                    </label>
                                </div>';
                }
                $xhtml .= '</div>';
                break;
            default:
                $xhtml = '<div class="btn-group" data-toggle="buttons">';
                foreach ($options as $valor => $descricao) {
                    $marcado = $value == $valor ? ' active' : '';
                    $xhtml .= ' <label class="btn btn-primary'. $marcado.'">
                                    ' . $this->montarInput($name, $valor, $descricao, $attribs, $config) . '
                                </label>';
                }
                $xhtml .= '</div>';
            break;
        }

        if ($help) {
        	$xhtml.= "<span class='help-block m-b-none'><i class='fa fa-question-circle' style='color: #1c84c6;'></i> {$attribs['help']}</span>";
        }
        
        return $this->_build($xhtml, $label, $config);
    }

    protected function montarInput($name, $valor, $descricao, $attribs = null, $config = null)
    {
        $id = isset($attribs['id']) ? $attribs['id'] : $name;
        $type = isset($attribs['type']) ? $attribs['type'] : 'radio';
        $required = is_array($config) && in_array('required', $config) ? 'required="required"' : '';
        $class = isset($attribs['class']) ? 'form-control ' .  $attribs['class'] : '';
        
        unset($attribs['id'], $attribs['type'], $attribs['class'], $attribs['help'], $attribs[0]);

		$xhtml = '<input'
        	   . ' name="' . $this->view->escape($name) . '"'
        	   . ' id="' . $this->view->escape($id) . '"'
        	   . ' type="' . $type . '"'
        	   . ' value="' . $valor . '"'
        	   . ' class="' . $class . '"'
        	   . ' ' . $required . ' '
        	   . $this->_htmlAttribs($attribs)
        	   . " />" . $descricao;
        
        return $xhtml;
    }

    protected function _build($xhtml, $label, $config)
    {
        $icon = !empty($config['icon']) ? '<span class="input-group-addon"><span class="glyphicon glyphicon-' . $config['icon'] . '"></span></span></span>' : null;
        $help = !empty($config['help']) ? '<span class="input-group-addon help-tooltip" data-toggle="tooltip" data-placement="left" title="' . $config['help'] . '"><span class="glyphicon glyphicon-question-sign"></span></span>' : null;
        $labelSize = !empty($config['label-size']) ? $config['label-size'] : 2;
        $inputSize = !empty($config['input-size']) ? $config['input-size'] : 10;
        
        if(isset($config['formTipo']) && $config['formTipo'] == Simec_View_Helper::K_FORM_TIPO_VERTICAL){
        	$classLabel = '';
        	$classInput = ' ' . $date;
        } else if ($labelSize || $inputSize) {
        	$classLabel = "col-sm-{$labelSize} col-md-{$labelSize} col-lg-{$labelSize} control-label";
        	$classInput = "col-sm-{$inputSize} col-md-{$inputSize} col-lg-{$inputSize} " . $date;
        } else {
        	$classLabel = 'col-sm-2 col-md-2 col-lg-2 control-label';
        	$classInput = 'col-sm-10 col-md-10 col-lg-10 ' . $date;
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
            $required = is_array($config) && in_array('required', $config) ? '<span class="campo-obrigatorio" title="Campo obrigatório">*</span>' : '';
            $xhtml =  '
                <div class="form-group">
                    <label for="intcnpj" class="'. $classLabel . ' control-label">' . $label . ': ' . $required . '</label>
                    <div class="' . $classInput . '">
                        '. $xhtml .'
                    </div>
                </div>
            ';
        }

        return $xhtml;
    }
}
