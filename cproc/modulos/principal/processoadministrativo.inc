<?php

include  APPRAIZ."includes/cabecalho.inc";

$titulo_modulo = "Processo Administrativo";
$modulo = "";
monta_titulo( $titulo_modulo, $modulo );

// dependências --------------------------------------------------------------------------------------------------
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include_once APPRAIZ . "cproc/classes/ModeloCproc.class.inc";
include_once APPRAIZ . "cproc/classes/Processo.class.inc";
include_once APPRAIZ . "cproc/classes/WorkflowCproc.class.inc";
// --------------------------------------------------------------------------------------------------
include_once '../../cproc/classes/ProcessoAdministrativo.class.inc';


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


$ProcessoAdministrativo = new ProcessoAdministrativo();

if( $_GET['padid'] ){

	if( $_GET['excluir'] ){
		$ProcessoAdministrativo->excluir( $_GET['padid'] );
		if( $ProcessoAdministrativo->commit() ){
			echo "<script>
				alert('Elemento excluído com sucesso.');
				window.location.href = 'cproc.php?modulo=principal/processoadministrativo&acao=A';
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

	$ProcessoAdministrativo->carregar( $_GET['padid'] );
}

if( $_POST['tipoForm'] ):

	if( $_POST['tipoForm'] == 'filtro' ){ // ------------------------------- filtro

		// -- filtros --
		$filtros = array();
		foreach( $_POST as $chave => $valor )
			if( $valor != '' )
				switch ($chave) {
					case 'padinstaurado_filtro':
						$filtros['padinstaurado'] = $valor;
						break;
				}
		// -- / filtros --

	}else if( $_POST['tipoForm']  == 'edicao' ){ // ------------------------------- edicao

		// recepcao de dados
		$campos = array();
		unset($campos['iesid']);
		unset($campos['co_mantenedora']);
		unset($campos['prcnumsidoc']);
		$campos['padid'] = $_POST['padid_campo'];
		$campos['prcid'] = $_POST['prcid_campo'];
	    $campos['tprid'] = $_POST['tprid_campo'];
	    $campos['padinstaurado'] = $_POST['padinstaurado_campo'];
	    $campos['padlinkdou'] = $_POST['padlinkdou'];
		$campos['paddtinstauracao'] = $_POST['paddtinstauracao'] ? $_POST['paddtinstauracao'] : null;
		$campos['paddtpublicacao'] = $_POST['paddtpublicacao'] ? $_POST['paddtpublicacao'] : null;
		$campos['paddtrevogacao'] = $_POST['paddtrevogacao'] ? $_POST['paddtrevogacao'] : null;
		$campos['paddtsuspensao'] = $_POST['paddtsuspensao'] ? $_POST['paddtsuspensao'] : null;
		$campos['paddtarquivamento'] = $_POST['paddtarquivamento'] ? $_POST['paddtarquivamento'] : null;
	    $campos['padobservacoes'] = $_POST['padobservacoes_campo'];

	    if( !$campos['padid'] ){ // insert

	    	// ver($campos,d);
	    	// $ProcessoAdministrativo->cadastrar($campos);exit;
	    	if( $ProcessoAdministrativo->cadastrar($campos) ){
	    		echo ' <script>
	    				alert("Cadastrado com sucesso.");
	    				location.href = "cproc.php?modulo=principal/processoadministrativo&acao=A";
	    			</script> ';
	    		exit;
	    	}else{
	    		echo ' <script>
	    				alert("Erro ao cadastrar.");
	    				location.href = "cproc.php?modulo=principal/processoadministrativo&acao=A";
	    			</script> ';
	    		exit;
	    	}

	    }else{ // update
	    	// dbg($campos,d);
	    	$ProcessoAdministrativo->popula( $campos );
	    	if( $ProcessoAdministrativo->atualizar() ){
	    		echo ' <script>
	    				alert("Atualizado com sucesso.");
	    				location.href = "cproc.php?modulo=principal/processoadministrativo&acao=A&padid='.($ProcessoAdministrativo->padid).'";
	    			</script> ';
	    		exit;
	    	}else{
	    		echo ' <script>
	    				alert("Existem campos obrigatórios em branco.");
	    				history.back();
	    			</script> ';
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
	<h4>Dados do Processo Administrativo:</h4>
	
	<div>

		<form method="post" name="formProcessoAdministrativo">
		<input name="tipoForm" id="tipoForm" value="edicao" type="hidden"/>
		<input name="padid_campo" id="padid_campo" type="hidden" value="<?=($ProcessoAdministrativo->padid)?$ProcessoAdministrativo->padid:''?>"/>
		<input name="prcid_campo" id="prcid_campo" type="hidden" value="<?=$processo['prcid']?>"/>
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
	        <tr>
	            <td class="subtituloDireita" colspan="2">Nº do Processo:</td>
	            <td>
	                <?php
	                    echo campo_texto('prcnumsidoc_campo', 'S', 'N', 'Nº Controle ' SIGLA_SISTEMA, '51', '25', '####.######/####-##', '', '', '', '', 'id="prcid_campo"', '', $processo['prcnumsidoc'], '');
	                ?>
	            </td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Mantenedora:</td>
	            <td>
	                <?PHP
	                    $sql = "
	                        SELECT  co_mantenedora AS codigo,
	                                no_razao_social AS descricao
	                        FROM emec.mantenedora
	                        ORDER BY no_razao_social
	                    ";
	                    $db->monta_combo('mntid', $sql, 'N', 'Selecione...', '', '', '', '455', 'S', '', '',$processo['mntid'],'Tipo de Processo');
	                ?>
	            </td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Mantida:</td>
	            <td>
	                <?PHP
	                   	$sql = "
	                        SELECT  co_ies AS codigo,
	                                no_ies AS descricao
	                        FROM emec.ies
	                        ORDER BY no_ies
	                    ";
	                    $db->monta_combo('iesid', $sql, 'N', 'Selecione...', '', '', '', '455', 'S', '', '',$processo['iesid'],'Tipo de Processo');
	                ?>
	            </td>
	        </tr>
	        <tr>
	        	<td class="subtituloDireita" colspan="2">Entidade:</td>
	        	<td>
	        		<?php 
	        			$sql = "
								SELECT
									entid AS codigo,
									entdsc AS descricao
								FROM cproc.entidade
								";
	        			$db->monta_combo('entid', $sql, 'N', 'Selecione...', '', '', '', '455', 'S', '', '',$processo['entid'],'Tipo de Processo');
	        		?>
	        	</td>
	        </tr>
                <?php responsaveisProcesso($processo['prcid'])?>
	        <tr style="height:20px;"></tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Processo Administrativo Instaurado: </td>
	            <td>
	                <input type="radio" name="padinstaurado_campo" id="padinstaurado_campo_sim" value="1" <?=($ProcessoAdministrativo->padinstaurado && $ProcessoAdministrativo->padinstaurado == 't')?'checked':''?> <?=($habil=='S')?'':'disabled' ?> />Sim
	                &nbsp;&nbsp;&nbsp;
	                <input type="radio" name="padinstaurado_campo" id="padinstaurado_campo_nao" value="0" <?=($ProcessoAdministrativo->padinstaurado && $ProcessoAdministrativo->padinstaurado == 'f')?'checked':''?> <?=($habil=='S')?'':'disabled' ?> />Não
	            </td>
	        </tr>
			<tr>
				<td class="subtituloDireita" colspan="2">Data de Instauração do PA:</td>
				<td>
					<?php
					$paddtinstauracao = ($ProcessoAdministrativo->paddtinstauracao)?$ProcessoAdministrativo->paddtinstauracao:'';
					echo campo_data2('paddtinstauracao', 'S', $habil, 'Data de Instauração do PA', 'S', '', '', $paddtinstauracao, '', '', 'paddtinstauracao');
					?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita" colspan="2">Data de Publicação no DOU:</td>
				<td>
					<?php
					$paddtpublicacao = ($ProcessoAdministrativo->paddtpublicacao)?$ProcessoAdministrativo->paddtpublicacao:'';
					echo campo_data2('paddtpublicacao', 'S', $habil, 'Data de Publicação no DOU', 'S', '', '', $paddtpublicacao, '', '', 'paddtpublicacao');
					?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita" colspan="2">Data de Revogação do PA:</td>
				<td>
					<?php
					$paddtrevogacao = ($ProcessoAdministrativo->paddtrevogacao)?$ProcessoAdministrativo->paddtrevogacao:'';
					echo campo_data2('paddtrevogacao', 'S', $habil, 'Data de Revogação do PA', 'S', '', '', $paddtrevogacao, '', '', 'paddtrevogacao');
					?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita" colspan="2">Data e Suspensão do PA:</td>
				<td>
					<?php
					$paddtsuspensao = ($ProcessoAdministrativo->paddtsuspensao)?$ProcessoAdministrativo->paddtsuspensao:'';
					echo campo_data2('paddtsuspensao', 'S', $habil, 'Data e Suspensão do PA', 'S', '', '', $paddtsuspensao, '', '', 'paddtsuspensao');
					?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita" colspan="2">Data e Arquivamento do Processo:</td>
				<td>
					<?php
					$paddtarquivamento = ($ProcessoAdministrativo->paddtarquivamento)?$ProcessoAdministrativo->paddtarquivamento:'';
					echo campo_data2('paddtarquivamento', 'S', $habil, 'Data e Arquivamento do Processo', 'S', '', '', $paddtarquivamento, '', '', 'paddtarquivamento');
					?>
				</td>
			</tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Link DOU:</td>
	            <td>
	                <?php
	                	$padlinkdou = ($ProcessoAdministrativo->padlinkdou)?$ProcessoAdministrativo->padlinkdou:'';
	                    echo campo_texto('padlinkdou', 'N', $habil, 'Link DOU', '51', '100', '', '', '', '', '', 'id="padlinkdou"', '', $ProcessoAdministrativo->padlinkdou, '');
	                ?>
	            </td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Observações:</td>
	            <td>
	                <textarea style="width:300px;height:150px" <?=($habil=='S')?'':'disabled' ?> name="padobservacoes_campo" id="padobservacoes_campo"><?=($ProcessoAdministrativo->padobservacoes)?$ProcessoAdministrativo->padobservacoes:'';?></textarea>
	            </td>
	        </tr>
	        <tr>
	            <td class="SubTituloCentro" colspan="3">
	            	<?php if ($habil=='S') { ?>
		                <input type="button" name="salvarProcesso" id="salvarProcesso" value="<?=( $_GET['padid'] )?'Salvar':'Cadastrar';?>" onclick="salvar();"/>
		                &nbsp;&nbsp;&nbsp;
		                <input name="reset" type="reset" value="Limpar campos"/>
		                <?php if( $_GET['padid'] ){ ?>
		                	&nbsp;&nbsp;&nbsp;
		                	<input name="novo" type="button" value="Cadastrar Novo Processo Administrativo" onclick="javascript:window.location.href='cproc.php?modulo=principal/processoadministrativo&acao=A'"/>
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
	            <td class="subtituloDireita" colspan="2">Processo Administrativo Instaurado: </td>
	            <td>
	                <input type="radio" name="padinstaurado_filtro" id="padinstaurado_filtro_sim" value="1" <?=($filtros['padinstaurado' ] && $filtros['padinstaurado' ] == 't')?'checked':''?> />Sim
	                &nbsp;&nbsp;&nbsp;
	                <input type="radio" name="padinstaurado_filtro" id="padinstaurado_filtro_nao" value="0" <?=($filtros['padinstaurado' ] && $filtros['padinstaurado' ] == 'f')?'checked':''?> />Não
	                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
	                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
	                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
	                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
	                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
	                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
	            </td>
	        </tr>
			<!-- tr>
	            <td class="subtituloDireita" colspan="2">SIDOC:</td>
	            <td> <input name="padsidoc_filtro" id="padsidoc_filtro" value="<?=@$filtros['padsidoc']?>" /> </td>
	        </tr -->
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
		$cabecalho = array("Ação","Sidoc","Data de Instauração do PA","Data de Publicação no DOU","Data de Revogação do PA","Data e Suspensão do PA","Data e Arquivamento do Processo","Observações");
		$alinhamento = array('left','left','center','center','center','center','center','left');
		$larguras = array('10%','10%','10%','10%','10%','10%','10%','30%');
		// ver($ProcessoAdministrativo->montaListaQuery( $filtros ),d);
		$filtros['prcid'] = $processo['prcid'];
		$db->monta_lista($ProcessoAdministrativo->montaListaQuery( $filtros ),$cabecalho,30,5,'N','','N','listaProcessoAdministrativo', $larguras,$alinhamento);
		?>
	</div>

</div>

<script>

function salvar(){

	if (!document.getElementById('padinstaurado_campo_sim').checked && !document.getElementById('padinstaurado_campo_nao').checked) {
		alert('Por favor enforme se o processo administrativo foi instaurado.');
		return false;
	}
	
	document.forms.formProcessoAdministrativo.submit();
}

function editar( chave ){
	window.location.href = "cproc.php?modulo=principal/processoadministrativo&acao=A&padid="+chave;
}

function excluir( chave ){
	if( confirm('Deseja apagar esse item?') ){
		window.location.href = "cproc.php?modulo=principal/processoadministrativo&acao=A&excluir=1&padid="+chave;
	}
}

jQuery(document).ready(function(){
	jQuery("[name='padinstaurado']").click(function(){
		var valor = jQuery(this).val();

		//if( valor == 0 ){ jQuery("[name='padsidoc_campo']").attr('disabled','disabled').val(''); }
		if( valor == 1 ){ jQuery("[name='padsidoc_campo']").removeAttr('disabled'); }

	});
});

</script>