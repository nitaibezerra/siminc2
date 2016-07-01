<?php
header('content-type: text/html; charset=iso-8859-1;');
include APPRAIZ . 'includes/cabecalho.inc';

print "<br/>";

// Monta as Abas da Página
$menu[0] = array("descricao" => "Itens de auditoria", "link"=> "fabrica.php?modulo=sistema/geral/itens-auditoria/formulario&acao=A");
echo montarAbasArray($menu, "fabrica.php?modulo=sistema/geral/itens-auditoria/formulario&acao=A");

//Monta o Titulo da página abaixo da Aba
monta_titulo("Incluir itens de auditoria", '');

?>
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet" />
<link href="./css/fabrica.css" type="text/css" rel="stylesheet" />

<script type="text/javascript" src="./js/jquery-1.7.min.js"></script>
<script type="text/javascript" src="./js/jquery.validate.min.js"></script>
<script type="text/javascript" src="./js/additional-methods.min.js"></script>

<form id="form-itensAuditoria" method="post" action="">

	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloDireita" width="25%">Item: </td>
			<td>
				<? echo campo_texto('itemAuditoria', 'S',  'S', "", 50, 30, '',
                    '', '', '', 0, 'id="itemAuditoria"', '', "", '', ''); ?>
			</td>
		</tr>
		<tr>
            <td class="SubTituloDireita">Descrição: </td>
            <td>
                <?php
	                $numeroDeLinhas = 65;
    	            $numeroDeColunas = 10;
    	            $maximoCaracteres = 250;
    	            $textoPadrao = "";
                ?>
                <span id="textAreaDinamico">
                	<?php echo campo_textarea('descricaoItemAuditoria', 'N', $habil, null, $numeroDeLinhas, $numeroDeColunas , $maximoCaracteres, null, null, null, null, null,$textoPadrao); ?>
                </span>
            </td>
        </tr>
		<tr>
			<td class="SubtituloDireita" width="25%">Situação:</td>
			<td>
				<input id="itensAuditoriaSituacao" type="checkbox" name="situacaoItemAuditoria" value="A" checked="checked"/>Ativo
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" width="25%"></td>
			<td>
				<input id="botaoSalvarNovoItemAuditoria" type="submit" value="Salvar"/>
				<input type="button" value="Cancelar" onclick="parent.location='?modulo=sistema/geral/itens-auditoria/listar&acao=A'"/>
			</td>
		</tr>
	</table>
</form>

<script type="text/javascript" src="./js/itens-auditoria.js"></script>