<?php 

function inputTextArea($nome, $valor, $id, $limite, $opcoes = array(), $bpClass = true) {
	$opcoesPadrao = array(
			'obrig' => 'N',
			'habil' => 'S',
			'label' => '',
			'cols' => null,
			'rows' => 4,
			'funcao' => '',
			'acao' => 0,
			'txtdica' => '',
			'tab' => false,
			'title' => null,
			'width' => null,
			'id' => null
	);

	// -- Extraíndo as opções solicitadas
	extract($opcoes, EXTR_OVERWRITE);
	// -- Extraíndo as opções padrão (apenas as que não foram definidas em $opcoes)
	extract($opcoesPadrao, EXTR_SKIP);

	$html = campo_textarea(
			$nome, $obrig, $habil, $label, $cols, $rows, $limite, $funcao, $acao, $txtdica, $tab, $title, $valor, $width, $id
	);

	// -- Javascript de formatação do campo de textarea
	$html .= <<<JAVASCRIPT
<script type="text/javascript" lang="javascript">
$(document).ready(function(){
    $('#{$id}').addClass('form-control').next().remove();
    $('#no_{$id}').addClass('form-control').css('width', '70px').css('margin-top', '5px').next().remove();
JAVASCRIPT;
	if ($complemento && is_array($complemento)) {
		foreach ($complemento as $comp => $valor) {
			switch ($comp) {
				case 'readonly':
				case 'disabled':
				case 'required':
					$html .= <<<JAVASCRIPT
    $('#{$id}').prop('{$comp}', true);
JAVASCRIPT;
					break;
				default:
					$html .= <<<JAVASCRIPT
    $('#{$id}').attr('{$comp}', '{$valor}');
JAVASCRIPT;
			}
		}
	}
	$html .= <<<JAVASCRIPT
});
</script>
JAVASCRIPT;

	if ($opcoes['return']) {
		return $html;
	}

	echo $html;
}
