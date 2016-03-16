<?php
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Relatório - Monitoramento de Obras', 'Relatório Validações' );

$dir_relatorios = APPRAIZ . 'arquivos/obras2/relatorio/validacao';
if($_GET['download']){
    if(file_exists($dir_relatorios . '/' . $_GET['download'])) {
        ob_clean();
        header("Content-Type: application/vnd.ms-excel; charset=ISO-8859-1");
        header("Content-Disposition: inline; filename=\"" . $_GET['download'] . "\"");
        echo file_get_contents($dir_relatorios . '/' . $_GET['download']);
    }
    exit;
}


?>



<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>

<form action="obras2.php?modulo=relatorio/popupRelatorioValidacoes&acao=A" method="post" name="filtro" id="filtro">
<input type="hidden" name="pesquisa" id="pesquisa" value="1"/>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
<!-- <tr>
	<td class="SubTituloDireita">Tipo de Ensino</td>
	<td>
		<?php						
			$orgaos = $db->carregar("SELECT orgid, orgdesc FROM obras2.orgao where orgid in (1, 2, 3)");					
			$count = count($orgaos);
			for($i = 0; $i < $count; $i++){						
				echo '<input type="checkbox" id="orgid" name="orgid[]" value="' . $orgaos[$i]['orgid'] . '"/> ' . $orgaos[$i]["orgdesc"] . '</label>&nbsp;';						
			}
		?>
	</td>
</tr>-->
	<?php
	// Programa
	$stSql = "SELECT
                        prfid AS codigo,
                        prfdesc AS descricao
                 FROM 
                        obras2.programafonte
                 ORDER BY
                        prfdesc ";
	mostrarComboPopup( 'Programa', 'prfid',  $stSql, $stSqlCarregados, 'Selecione o(s) Programa(s)' );
	
	// Fonte
	$stSql = "SELECT
                        tooid AS codigo,
                        toodescricao AS descricao
                 FROM 
                        obras2.tipoorigemobra
                 WHERE
                        toostatus = 'A'
                 ORDER BY
                        toodescricao ";
	
	$sql_carregados = "	SELECT
                                        tooid AS codigo,
                                        toodescricao AS descricao
                                FROM 
                                        obras2.tipoorigemobra
                                WHERE
                                        toostatus = 'A'
                                AND
                                        tooid = 1
                                ORDER BY
                                        toodescricao ";
	
	mostrarComboPopup( 'Fonte', 'tooid',  $stSql, $sql_carregados, 'Selecione a(s) Fonte(s)' );
?>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Tipologia da Obra:</td>
        <td>
            <?php
            $tipologiaObra = new TipologiaObra();
            $param = array("orgid" => $_SESSION['obras2']['orgid']);
            $db->monta_combo("tpoid", $tipologiaObra->listaCombo($param), "S", "Todas", "", "", "", 200, "N", "tpoid");
            ?>
        </td>
    </tr>
<tr>
	<td class="SubTituloDireita" style="width: 190px;">Unidade:</td>
	<td>
		<?php 			
			$sql = "SELECT 
                                        ee.entid as codigo, 
                                        upper(ee.entnome) as descricao 
                                FROM
                                        entidade.entidade ee
                                INNER JOIN 
                                        obras2.empreendimento oi ON oi.entidunidade = ee.entid 
                                WHERE
                                        orgid = 3 AND
                                        empstatus = 'A'
                                GROUP BY 
                                        ee.entnome, 
                                        ee.entid 
                                ORDER BY 
                                        ee.entnome";
		
			$db->monta_combo( "entidunidade", $sql, "S", "Todos", "", "", "", "550", "N", "entidunidade" );
			
		?>
	</td>
</tr>

<tr>
	<td class="SubTituloDireita" style="width: 190px;">Situação da Obra:</td>
	<td>
		<?php 
			$sql = "SELECT 
                                        esdid as codigo, 
                                        esddsc as descricao 
                                FROM 
                                        workflow.estadodocumento 
                                WHERE
                                        esdstatus = 'A' AND tpdid = '".TPDID_OBJETO."'
                                ORDER BY 
                                        esddsc";
		
			$db->monta_combo( "esdid", $sql, "S", "Todas", "", "", "", "", "N", "esdid" );			
		?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" style="width: 190px;">UF:</td>
	<td>
		<?php 
			$sql = "SELECT
                                        estuf as codigo,
                                        estdescricao as descricao
                                FROM
                                        territorios.estado
                                ORDER BY
                                        estdescricao";
		
			$db->monta_combo( "estuf", $sql, "S", "Todas", "", "", "", "", "N", "estuf" );			
		?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" style="width: 190px;">ID Obras</td>
	<td>
		<?php echo campo_texto( 'obrid', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, ''); ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" style="width: 190px;">ID Pré-Obra:</td>
	<td>
		<?php echo campo_texto( 'preid', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, ''); ?>
	</td>
</tr>
<tr>
                        <td class="SubTituloDireita" style="width: 190px;">Recebeu o primeiro repasse?</td>
                        <td>
                            <input type="radio" name="repasse" id="" value="S" <?= ( $_POST["repasse"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
                            <input type="radio" name="repasse" id="" value="N" <?= ( $_POST["repasse"] == "N" ? "checked='checked'" : "" ) ?>/> Não
                            <input type="radio" name="repasse" id="" value="T"  <?= ( $_POST["repasse"] == "T" || $_POST["repasse"] == "" ? "checked='checked'" : "" ) ?> /> Todas
                        </td>
                    </tr>
<tr>
	<td class="SubTituloDireita">2ª Parcela Validada:</td>
	<td>
		<input type="radio" value="S" id="homologacao" name="homologacao" <?php if($_REQUEST["homologacao"] == "S") { echo "checked"; } ?> /> Sim
		<input type="radio" value="N" id="homologacao" name="homologacao" <?php if($_REQUEST["homologacao"] == "N") { echo "checked"; } ?> /> Não Validados
		<input type="radio" value="P" id="homologacao" name="homologacao" <?php if($_REQUEST["homologacao"] == "P") { echo "checked"; } ?> /> Não Analisados
		<input type="radio" value="" id="homologacao" name="homologacao"  <?php if($_REQUEST["homologacao"] == "") { echo "checked"; } ?> /> Todos
	</td>
</tr>
<tr>
    <td class="subtitulodireita" width="190px">Data 2ª Parcela Validada:</td>
    <td>
        de: <input type="text" id="dthomologacao_de" name="dthomologacao_de" value="<?php echo $dthomologacao_de;?>" size="15" maxlength="10" class="normal calendar" >
        &nbsp;
        até: <input type="text" id="dthomologacao_ate" name="dthomologacao_ate" value="<?php echo $dthomologacao_ate;?>" size="15" maxlength="10" class="normal calendar">
    </td>
</tr>
<tr>
	<td class="SubTituloDireita">Execução 25% Validada:</td>
	<td>
		<input type="radio" value="S" id="execucao25" name="execucao25" <?php if($_REQUEST["execucao25"] == "S") { echo "checked"; } ?> /> Sim
		<input type="radio" value="N" id="execucao25" name="execucao25" <?php if($_REQUEST["execucao25"] == "N") { echo "checked"; } ?> /> Não Validados
		<input type="radio" value="P" id="execucao25" name="execucao25" <?php if($_REQUEST["execucao25"] == "P") { echo "checked"; } ?> /> Não Analisados
		<input type="radio" value="" id="execucao25" name="execucao25"  <?php if($_REQUEST["execucao25"] == "") { echo "checked"; } ?> /> Todos
	</td>
</tr>
<tr>
    <td class="subtitulodireita" width="190px">Data 2ª Execução 25% Validada:</td>
    <td>
        de: <input type="text" id="dtexecucao25_de" name="dtexecucao25_de" value="<?php echo $dtexecucao25_de;?>" size="15" maxlength="10" class="normal calendar" >
        &nbsp;
        até: <input type="text" id="dtexecucao25_ate" name="dtexecucao25_ate" value="<?php echo $dtexecucao25_ate;?>" size="15" maxlength="10" class="normal calendar">
    </td>
</tr>
<tr>
	<td class="SubTituloDireita">Execução 50% Validada:</td>
	<td>
		<input type="radio" value="S" id="execucao50" name="execucao50" <?php if($_REQUEST["execucao50"] == "S") { echo "checked"; } ?> /> Sim
		<input type="radio" value="N" id="execucao50" name="execucao50" <?php if($_REQUEST["execucao50"] == "N") { echo "checked"; } ?> /> Não Validados
		<input type="radio" value="P" id="execucao50" name="execucao50" <?php if($_REQUEST["execucao50"] == "P") { echo "checked"; } ?> /> Não Analisados
		<input type="radio" value="" id="execucao50" name="execucao50"  <?php if($_REQUEST["execucao50"] == "") { echo "checked"; } ?> /> Todos
	</td>
</tr>
<tr>
    <td class="subtitulodireita" width="190px">Data Execução 50% Validada:</td>
    <td>
        de: <input type="text" id="dtexecucao50_de" name="dtexecucao50_de" value="<?php echo $dtexecucao50_de;?>" size="15" maxlength="10" class="normal calendar" >
        &nbsp;
        até: <input type="text" id="dtexecucao50_ate" name="dtexecucao50_ate" value="<?php echo $dtexecucao50_ate;?>" size="15" maxlength="10" class="normal calendar">
    </td>
</tr>
<tr>
	<td class="SubTituloDireita">Possui anexo da homologação da licitação:</td>
	<td>
		<input type="radio" value="S" id="arqhanexosim" name="arqhanexo"  <?php if($_REQUEST["arqhanexo"] == "S") { echo "checked"; } ?> /> Sim
		<input type="radio" value="N" id="arqhanexonao" name="arqhanexo"  <?php if($_REQUEST["arqhanexo"] == "N") { echo "checked"; } ?> /> Não		
		<input type="radio" value="" id="arqahnexotodos" name="arqhanexo" <?php if($_REQUEST["arqhanexo"] == "") { echo "checked"; } ?> /> Todos
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Possui anexo da ordem de serviço:</td>
	<td>
		<input type="radio" value="S" id="arqanexosim" name="arqanexo"  <?php if($_REQUEST["arqanexo"] == "S") { echo "checked"; } ?> /> Sim
		<input type="radio" value="N" id="arqanexonao" name="arqanexo"  <?php if($_REQUEST["arqanexo"] == "N") { echo "checked"; } ?> /> Não		
		<input type="radio" value="" id="arqanexotodos" name="arqanexo" <?php if($_REQUEST["arqanexo"] == "") { echo "checked"; } ?> /> Todos
	</td>
</tr>
<tr>
	<td bgcolor="#CCCCCC"></td>
	<td bgcolor="#CCCCCC">
		<input type="button" value="Visualizar" onclick="obras_exibeRelatorioGeral('exibir');" style="cursor: pointer;"/>
		<input type="button" value="Visualizar XLS" onclick="obras_exibeRelatorioGeral('xls');" style="cursor: pointer;"/>
        <?
        $dir_relatorios = APPRAIZ . 'arquivos/obras2/relatorio/validacao';

        if(file_exists($dir_relatorios)) {
            $scanned_directory = array_diff(scandir($dir_relatorios), array('..', '.', '.svn'));

            $file = array_pop($scanned_directory);
            if ($file):
                ?>
                <input type="button" value="Donwload Relatório Compilado"
                       onclick="downloadUltimoArquivo('<?= $file ?>');" style="cursor: pointer;"/>
            <? endif; ?>
        <?
        }
        ?>
	</td>
</tr>
</table>
<script type="text/javascript">
var formulario = document.getElementById('filtro');

function obras_exibeRelatorioGeral(tipo){
			 
	/*if ( !document.getElementsByName('orgid[]').item(0).checked &&
		 !document.getElementsByName('orgid[]').item(1).checked &&
		 !document.getElementsByName('orgid[]').item(2).checked ){
		alert( 'Favor selecionar ao menos um tipo de ensino!' );
		return false;
	}*/
	
	if(document.getElementById('prfid_campo_flag').value == "1"){
		selectAllOptions( document.getElementById( 'prfid' ) );
	}
	if(document.getElementById('tooid_campo_flag').value == "1"){
		selectAllOptions( document.getElementById( 'tooid' ) );
	}
	
	formulario.target = 'RelatorioValidacoes';
	var janela = window.open( 'obras2.php?modulo=relatorio/popupRelatorioValidacoes&acao=A', 'RelatorioValidacoes', 'status=no,menubar=no,toolbar=no,scrollbars=1,resizable=no,fullscreen=yes' );
	janela.focus();
	
	// Tipo de relatorio
	document.getElementById('pesquisa').value= tipo ;
	formulario.submit();		
}

/**
	 * Alterar visibilidade de um bloco.
	 * 
	 * @param string indica o bloco a ser mostrado/escondido
	 * @return void
	 */
	function onOffBloco( bloco )
	{
		var div_on = document.getElementById( bloco + '_div_filtros_on' );
		var div_off = document.getElementById( bloco + '_div_filtros_off' );
		var img = document.getElementById( bloco + '_img' );
		var input = document.getElementById( bloco + '_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '0';
			img.src = '/imagens/menos.gif';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '1';
			img.src = '/imagens/mais.gif';
		}
	}
	
	/**
	 * Alterar visibilidade de um campo.
	 * 
	 * @param string indica o campo a ser mostrado/escondido
	 * @return void
	 */
	function onOffCampo( campo )
	{
		var div_on = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input = document.getElementById( campo + '_campo_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}
</script>
</form>

<script type="text/javascript">

    $(function(){

        jQuery(function($){
            $.datepicker.regional['pt-BR'] = {
                closeText: 'Fechar',
                prevText: '&#x3c;Anterior',
                nextText: 'Pr&oacute;ximo&#x3e;',
                currentText: 'Hoje',
                monthNames: ['Janeiro','Fevereiro','Mar&ccedil;o','Abril','Maio','Junho','Julho','Agosto','Setembro','Outubro','Novembro','Dezembro'],
                monthNamesShort: ['Jan','Fev','Mar','Abr','Mai','Jun','Jul','Ago','Set','Out','Nov','Dez'],
                dayNames: ['Domingo','Segunda-feira','Ter&ccedil;a-feira','Quarta-feira','Quinta-feira','Sexta-feira','S&aacute;bado'],
                dayNamesShort: ['Dom','Seg','Ter','Qua','Qui','Sex','Sab'],
                dayNamesMin: ['Dom','Seg','Ter','Qua','Qui','Sex','Sab'],
                weekHeader: 'Sm',
                dateFormat: 'dd/mm/yy',
                firstDay: 0,
                isRTL: false,
                showMonthAfterYear: false,
                yearSuffix: ''};
            $.datepicker.setDefaults($.datepicker.regional['pt-BR']);
        });


        jQuery('.calendar').mask('99/99/9999');
        jQuery('.calendar').datepicker({
            numberOfMonths: 1,
            dateFormat: 'dd/mm/yy',
            showWeek: true,
            showAnim:'drop'
        });
    });
</script>

<script type="text/javascript">
    function downloadUltimoArquivo(file){
        window.open( window.location + '&download=' + file );
    }
</script>