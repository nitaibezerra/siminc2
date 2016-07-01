<?php

include  APPRAIZ."includes/cabecalho.inc";

$titulo_modulo = "Verificação in Loco";
$modulo = "";
monta_titulo( $titulo_modulo, $modulo );

// dependências --------------------------------------------------------------------------------------------------
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include_once APPRAIZ . "cproc/classes/ModeloCproc.class.inc";
include_once APPRAIZ . "cproc/classes/Processo.class.inc";
include_once APPRAIZ . "cproc/classes/WorkflowCproc.class.inc";
// --------------------------------------------------------------------------------------------------
include_once '../../cproc/classes/VerificacaoInLoco.class.inc';

if( !verificaExistenciaPrcid() ){
	echo "
		<script>
			alert('Sessão perdeu processo.');
			window.location.href = 'cproc.php?modulo=principal/lista_grid_processo&acao=A';
		</script>";
	exit;	
}
 if ($_SESSION['cproc']['prcid']) {
         $processo = exibirProcesso($_SESSION['cproc']['prcid']);
         extract($processo);
    }
    else{
        $processo = exibirProcesso($prcid);
    }




$VerificacaoInLoco = new VerificacaoInLoco();

if( $_GET['vrlid'] ){

	if( $_GET['excluir'] ){
		$VerificacaoInLoco->excluir( $_GET['vrlid'] );
		if( $VerificacaoInLoco->commit() ){
			echo "<script>
				alert('Elemento excluído com sucesso.');
				window.location.href = 'cproc.php?modulo=principal/verificacaoinloco&acao=A';
			</script>";
			exit;
		}else{
			echo "<script>
				alert('Erro ao tentar excluir elemento.');
				window.history.back();
			</script>";
			exit;
		}
	}

	$VerificacaoInLoco->carregar( $_GET['vrlid'] );
}

if( $_POST['tipoForm'] ):

	if( $_POST['tipoForm'] == 'filtro' ){ // ------------------------------- filtro

		// -- filtros --
		$filtros = array();
		foreach( $_POST as $chave => $valor )
			if( $valor != '' )
				switch ($chave) {
					case 'vrltipo_filtro':
						$filtros['vrltipo'] = $valor;
						break;
					case 'vrldata_filtro':
						$filtros['vrldata'] = ajusta_data($valor);
						break;
					case 'vrlobservacoes_filtro':
						$filtros['vrlobservacoes'] = $valor;
						break;
				}
		// -- / filtros --

	}else if( $_POST['tipoForm']  == 'edicao' ){ // ------------------------------- edicao

		// recepcao de dados
		$campos = array();
		unset($campos['co_ies_campo']);
		unset($campos['co_mantenedora']);
		unset($campos['prcnumsidoc']);
		$campos['vrlid'] = $_POST['vrlid_campo'];
		$campos['prcid'] = $_POST['prcid_campo'];
		$campos['vrltipo'] = $_POST['vrltipo_campo'];
	    $campos['vrldata'] = ajusta_data($_POST['vrldata_campo']);
	    $campos['vrlobservacoes'] = $_POST['vrlobservacoes_campo'];
	    $campos['vrlnumprocemec'] = $_POST['vrlnumprocemec_campo'];

	    if( !$campos['vrlid'] ){ // insert

	    	// ver($campos,d);
	    	// $VerificacaoInLoco->cadastrar($campos);
	    	if( $VerificacaoInLoco->cadastrar($campos) ){
	    		echo "<script>
	    				alert('Cadastrado com sucesso.');
	    				window.location.href = 'cproc.php?modulo=principal/verificacaoinloco&acao=A';
	    			</script> ";
	    		exit;
	    	}else{
	    		echo "<script>
	    				alert('Erro ao cadastrar.');
	    				window.location.href = 'cproc.php?modulo=principal/verificacaoinloco&acao=A';
	    			</script> ";
	    		exit;
	    	}

	    }else{ // update

	    	$VerificacaoInLoco->popula( $campos );
	    	if( $VerificacaoInLoco->atualizar() ){
	    		echo " <script>
	    				alert('Atualizado com sucesso.');
	    				window.location.href = 'cproc.php?modulo=principal/verificacaoinloco&acao=A&vrlid=".($VerificacaoInLoco->vrlid)."';
	    			</script> ";
	    		exit;
	    	}else{
	    		echo " <script>
	    				alert('Existem campos obrigatórios em branco.');
	    				window.history.back();
	    			</script> ";
	    		exit;
	    	}
	    }

	}

endif;

$habil = retornaPermissaoDetalhe($prcid);
//$habil = 'S';

?>

<br>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>	
<script language="javascript" type="text/javascript">
	jQuery.noConflict();
	
	jQuery('.adicionar').live('click',function(){
		return window.open('assint.php?modulo=principal/cadCursoPEC_G&acao=A','modelo',"height=450,width=850,scrollbars=yes,top=50,left=200");
	});
</script>

<?php 
$arMnuid = array();
$db->cria_aba( ABA_CAD_PROCESSO, $url, '', $arMnuid );
?>

<div style="width:98%;margin:0 auto;">
	<h4>Dados da Verificação In Loco:</h4>
	
	<div>

		<form method="post" name="formVerificacaoInLoco">
		<input name="tipoForm" id="tipoForm" value="edicao" type="hidden"/>
		<input name="vrlid_campo" id="vrlid_campo" type="hidden" value="<?=($VerificacaoInLoco->vrlid)?$VerificacaoInLoco->vrlid:''?>"/>
		<input name="prcid_campo" id="prcid_campo" type="hidden" value="<?=$processo['prcid']?>"/>
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
	        <tr>
	            <td class="subtituloDireita" colspan="2">Nº do Processo:</td>
	            <td>
	                <?php
	                    echo campo_texto('prcnumsidoc_campo', 'S', 'N', 'Nº Controle SIMEC', '51', '25', '####.######/####-##', '', '', '', '', 'id="prcid_campo"', '', $processo['prcnumsidoc'], '');
	                ?>
	            </td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Mantenedora:</td>
	            <td>
	                <?php
	                    $sql = "
	                        SELECT  co_mantenedora AS codigo,
	                                no_razao_social AS descricao
	                        FROM emec.mantenedora
	                        ORDER BY no_razao_social
	                    ";
	                    $db->monta_combo('co_mantenedora_campo', $sql, 'N', 'Selecione...', '', '', '', '455', 'S', '', '',$processo['mntid'],'Tipo de Processo');
	                ?>
	            </td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Mantida:</td>
	            <td>
	                <?php
	                   	$sql = "
	                        SELECT  co_ies AS codigo,
	                                no_ies AS descricao
	                        FROM emec.ies
	                        ORDER BY no_ies
	                    ";
	                    $db->monta_combo('co_ies_campo', $sql, 'N', 'Selecione...', '', '', '', '455', 'S', '', '',$processo['iesid'],'Tipo de Processo');
	                ?>
	            </td>
	        </tr>
                <?php responsaveisProcesso($prcid)?>
	        <tr style="height:20px;"></tr>
			<tr>
	            <td class="subtituloDireita" colspan="2">Tipo:</td>
	            <td>
	                <?php
	                	$vrltipo = ($VerificacaoInLoco->vrltipo)?$VerificacaoInLoco->vrltipo:'';
	                	
	                	$sql = "SELECT tvlid as codigo, tvldsc as descricao FROM cproc.tipoverifinloco WHERE tvlstatus = 'A'";
	                	
	                	$db->monta_combo('vrltipo_campo', $sql, $habil, 'Selecione...', 'carregaNumProc', '', '', '455', 'S', '', '',$VerificacaoInLoco->vrltipo,'Tipo');
	                    //echo campo_texto('vrltipo_campo', 'S', $habil, 'Tipo', '51', '40', '', '', '', '', '', 'id="vrltipo_campo"', '', $vrltipo, '');
	                ?>
	            </td>
	        </tr>
	        <tr id="trNumProc" <?php echo trim($VerificacaoInLoco->vrlnumprocemec) == '' ? 'style="display:none"' : '' ?>>
	        	<td class="subtituloDireita" colspan="2" id="lblNumProc">
	        		<?php 
	        			if ($VerificacaoInLoco->vrltipo != ''){
							switch ($VerificacaoInLoco->vrltipo){
								case VRL_REGULACAO_INEP:
									echo 'Processo e-MEC:';
									break;
								case VRL_SUPERVISAO_INEP:
									echo 'Processo e-MEC:';
									break;
								case VRL_SUPERVISAO_INTERNA:
									echo 'Despacho Ordinário:';
									break;
							}
						}
	        		?>
	        	</td>
	        	<td>
	        		<?php 
	        			echo campo_texto('vrlnumprocemec_campo', 'N', $habil, 'Tipo', '51', '20', '', '', '', '', '', 'id="vrlnumprocemec_campo"', '', trim($VerificacaoInLoco->vrlnumprocemec), '');
	        		?>
	        	</td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Data:</td>
	            <td>
	                <?php 
	                	$vrldata = ($VerificacaoInLoco->vrldata)?$VerificacaoInLoco->vrldata:'';
	                	echo campo_data2('vrldata_campo', 'S', $habil, 'Data', 'S', '', '', $vrldata, '', '', 'vrldata_campo'); 
	                ?>
	            </td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Observações:</td>
	            <td>
	                <textarea style="width:300px;height:150px" <?=($habil=='S')?'':'disabled' ?> maxlength="400" name="vrlobservacoes_campo" id="vrlobservacoes_campo"><?=($VerificacaoInLoco->vrlobservacoes)?$VerificacaoInLoco->vrlobservacoes:'';?></textarea>
	            </td>
	        </tr>
	        <tr>
	            <td class="SubTituloCentro" colspan="3">
	            	<?php if ($habil=='S') { ?>
		                <input type="button" name="salvarProcesso" id="salvarProcesso" value="<?=( $_GET['vrlid'] )?'Salvar':'Cadastrar';?>" onclick="salvar();"/>
		                &nbsp;&nbsp;&nbsp;
		                <input name="reset" type="reset" value="Limpar campos"/>
		                <?php if( $_GET['vrlid'] ){ ?>
		                	&nbsp;&nbsp;&nbsp;
		                	<input name="novo" type="button" value="Cadastrar Nova Verificação In Loco" onclick="javascript:window.location.href='cproc.php?modulo=principal/verificacaoinloco&acao=A'"/>
		                <?php } ?>
	                <?php } ?>
	            </td>
	        </tr>
	    </table>
		</form>

	</div>

	<hr/>

	<div>
		<h4>Lista:</h4>

		<form method="post" name="form_filtro">
		<input name="tipoForm" id="tipoForm" value="filtro" type="hidden"/>
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
			<tr><td colspan="3"><h5>Filtros</h5></td></tr>
	      
	        <tr>
	            <td class="subtituloDireita" colspan="2">Tipo:</td>
	            <td>
	            	<?php
	            		$vrltipo_filtro = ($filtros['vrltipo'])?$filtros['vrltipo']:'';
	                    echo campo_texto('vrltipo_filtro', 'S', 'S', 'Tipo', '51', '25', '', '', '', '', '', 'id="vrltipo_filtro"', '', $vrltipo_filtro, '');
	                ?>
	            </td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Data:</td>
	            <td> 
	            	<?php 
	                	$vrldata_filtro = ($filtros['vrldata'])?$filtros['vrldata']:'';
	                	echo campo_data2('vrldata_filtro', 'S', 'S', 'Data', 'S', '', '', $vrldata_filtro, '', '', 'vrldata_filtro'); 
	                ?>
	           	</td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Observações:</td>
	            <td>
	                <?php
	            		$vrlindicarsidoc_filtro = ($filtros['vrlobservacoes'])?$filtros['vrlobservacoes']:'';
	                    echo campo_texto('vrlobservacoes_filtro', 'S', 'S', 'Observações', '51', '25', '', '', '', '', '', 'id="vrlobservacoes_filtro"', '', $vrlobservacoes_filtro, '');
	                ?>
	            </td>
	        </tr>
	        <tr>
	            <td class="SubTituloCentro" colspan="3">
	                <input type="submit" name="buscar" id="buscar" value="Buscar"/>
	                &nbsp;&nbsp;&nbsp;
	                <input name="reset" type="reset" value="Limpar campos"/>
	            </td>
	        </tr>
	    </table>
		</form>

		<?php 
		$cabecalho = array("Ação","Tipo","Data","Observação");
		$alinhamento = array('left','left','left','left');
		$larguras = array('10%','5%','20%','20%','40%');
		$filtros['prcid'] = $processo['prcid'];
		// ver($VerificacaoInLoco->montaListaQuery( $filtros ),d);
		$db->monta_lista($VerificacaoInLoco->montaListaQuery( $filtros ),$cabecalho,30,5,'N','','N','listaVerificacaoInLoco', $larguras,$alinhamento);
		?>
	</div>

</div>

<script>

function salvar(){
	document.forms.formVerificacaoInLoco.submit();
}

function editar( chave ){
	window.location.href = "cproc.php?modulo=principal/verificacaoinloco&acao=A&vrlid="+chave;
}

function excluir( chave ){
	if( confirm('Deseja apagar esse item?') ){
		window.location.href = "cproc.php?modulo=principal/verificacaoinloco&acao=A&excluir=1&vrlid="+chave;
	}
}

function carregaNumProc(tipo){
	switch(tipo){
		case '<?= VRL_REGULACAO_INEP ?>':
			jQuery('#trNumProc').removeAttr('style');
			jQuery('#lblNumProc').html('Processo e-MEC:');
			break;
		case  '<?= VRL_SUPERVISAO_INEP ?>':
			jQuery('#trNumProc').removeAttr('style');
			jQuery('#lblNumProc').html('Processo e-MEC:');
			break;
		case '<?= VRL_SUPERVISAO_INTERNA ?>':
			jQuery('#trNumProc').removeAttr('style');
			jQuery('#lblNumProc').html('Despacho Ordinário:');
			break; 
		default:
			jQuery('#trNumProc').attr('style','display: none');
			jQuery('#lblNumProc').html('');
			break;
	}
}

</script>