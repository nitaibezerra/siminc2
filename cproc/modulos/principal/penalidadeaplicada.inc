<?php

include  APPRAIZ."includes/cabecalho.inc";

$titulo_modulo = "Penalidade Aplicada";
$modulo = "";
monta_titulo( $titulo_modulo, $modulo );

// dependências --------------------------------------------------------------------------------------------------
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include_once APPRAIZ . "cproc/classes/ModeloCproc.class.inc";
include_once APPRAIZ . "cproc/classes/Processo.class.inc";
include_once APPRAIZ . "cproc/classes/WorkflowCproc.class.inc";
// --------------------------------------------------------------------------------------------------
include_once '../../cproc/classes/PenalidadeAplicada.class.inc';

if( !verificaExistenciaPrcid() ){
	echo '
		<script>
			alert("Sessão perdeu processo.");
			location.href = "cproc.php?modulo=principal/lista_grid_processo&acao=A";
		</script>';
	exit;	
}
 if ($_SESSION['cproc']['prcid']) {
         $processo = exibirProcesso($_SESSION['cproc']['prcid']);
         extract($processo);
}
else{
	$processo = exibirProcesso($prcid);
}

//verifica se existe pelo menos um Processo Administrativo para liberar a aba
$sql = "select count(padid) as total from cproc.processoadmin where prcid = ".$_SESSION['cproc']['prcid'] ;
$totalProcesso = $db->pegaUm($sql);
if($totalProcesso==0){
	echo '
		<script>
			alert("É necessário possuir pelo menos um Processo Administrativo cadastrado.");
			location.href = "cproc.php?modulo=principal/processoadministrativo&acao=A";
		</script>';
	exit;
}





$PenalidadeAplicada = new PenalidadeAplicada();

if( $_GET['papid'] ){

	if( $_GET['excluir'] ){
		$PenalidadeAplicada->excluir( $_GET['papid'] );
		if( $PenalidadeAplicada->commit() ){
			echo '<script>
				alert("Elemento excluído com sucesso.");
				location.href = "cproc.php?modulo=principal/penalidadeaplicada&acao=A";
			</script>';
			exit;
		}else{
			echo '<script>
				alert("Erro ao tentar excluir elemento.");
				window.history.back();
			</script>';
			exit;
		}
	}

	$PenalidadeAplicada->carregar( $_GET['papid'] );
}

if( $_POST['tipoForm'] ):

	if( $_POST['tipoForm'] == 'filtro' ){ // ------------------------------- filtro

		// -- filtros --
		$filtros = array();
		foreach( $_POST as $chave => $valor )
			if( $valor != '' )
				switch ($chave) {
					case 'vpavigencia_filtro':
						$filtros['vpavigencia'] = $valor;
						break;
					case 'paptipo_filtro':
						$filtros['paptipo'] = $valor;
						break;
					case 'papdataaplicacao_filtro':
						$filtros['papdataaplicacao'] = ajusta_data($valor);
						break;
					case 'paplinkdou_filtro':
						$filtros['paplinkdou'] = $valor;
						break;
				}
		// -- / filtros --

	}else if( $_POST['tipoForm']  == 'edicao' ){ // ------------------------------- edicao

		// recepcao de dados
		$campos = array();
		unset($campos['co_ies_campo']);
		unset($campos['co_mantenedora']);
		unset($campos['prcnumsidoc']);
		$campos['papid'] = $_POST['papid_campo'];
		$campos['prcid'] = $_POST['prcid_campo'];
		$campos['vpavigencia'] = $_POST['vpavigencia_campo'];
		$campos['opaid'] = $_POST['opaid'];
	    $campos['paptipo'] = ajusta_data($_POST['paptipo_campo']);
	    $campos['papdataaplicacao'] = ajusta_data($_POST['papdataaplicacao_campo']);
	    $campos['papobservacoes'] = $_POST['papobservacoes_campo'];
	    $campos['paplinkdou'] = $_POST['paplinkdou_campo'];
	    // vigencia
			$campos['vpajustificativa'] = $_POST['vpajustificativa'];
    		$campos['vpadtrevogacao'] = ajusta_data($_POST['vpadtrevogacao']);
    		$campos['vpadatanao'] = ajusta_data($_POST['vpadatanao']);

	    if( !$campos['papid'] ){ // insert

	    	// ver($campos,d);
	    	// $PenalidadeAplicada->cadastrar($campos);
	    	if( $PenalidadeAplicada->cadastrar($campos) ){
	    		echo '<script>
	    				alert("Cadastrado com sucesso.");
	    				location.href = "cproc.php?modulo=principal/penalidadeaplicada&acao=A";
	    			</script>';
	    		exit;
	    	}else{
	    		echo '<script>
	    				alert("Erro ao cadastrar.");
	    				location.href = "cproc.php?modulo=principal/penalidadeaplicada&acao=A";
	    			</script>';
	    		exit;
	    	}

	    }else{ // update

	    	$PenalidadeAplicada->popula( $campos );
	    	if( $PenalidadeAplicada->atualizar() ){

	    		if(!$campos['vpavigencia'])
	    			$campos['vpavigencia'] = 'false';

	    		$dadosVigencia = array( 
	    			'vpavigencia' => $campos['vpavigencia'],
	    			'vpaid' => $PenalidadeAplicada->vpaid,
	    			'vpajustificativa' => $campos['vpajustificativa'],
				    'vpadtrevogacao' => $campos['vpadtrevogacao'],
				    'vpadatanao' => $campos['vpadatanao'] );
	    		$PenalidadeAplicada->atualizaVigencia( $dadosVigencia );

	    		echo '<script>
	    				alert("Atualizado com sucesso.");
	    				location.href = "cproc.php?modulo=principal/penalidadeaplicada&acao=A&papid='.($PenalidadeAplicada->papid).'";
	    			</script>';
	    		exit;
	    	}else{
	    		echo '<script>
	    				alert("Existem campos obrigatórios em branco.");
	    				history.back();
	    			</script>';
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
	<h4>Dados da Penalidade Aplicada:</h4>
	
	<div>

		<form method="post" name="formPenalidadeAplicada">
		<input name="tipoForm" id="tipoForm" value="edicao" type="hidden"/>
		<input name="papid_campo" id="papid_campo" type="hidden" value="<?=($PenalidadeAplicada->papid)?$PenalidadeAplicada->papid:''?>"/>
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
	                    $db->monta_combo('mntid', $sql, 'N', 'Selecione...', '', '', '', '455', 'S', '', '',$processo['co_mantenedora'],'Tipo de Processo');
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
	                    $db->monta_combo('iesid', $sql, 'N', 'Selecione...', '', '', '', '455', 'S', '', '',$processo['co_ies'],'Tipo de Processo');
	                ?>
	            </td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Entidade:</td>
	            <td>
	                <?PHP
	                   	$sql = "
	                        SELECT  entid AS codigo,
	                                entdsc AS descricao
	                        FROM cproc.entidade
	                        ORDER BY entid
	                    ";
	                    $db->monta_combo('entid', $sql, 'N', 'Selecione...', '', '', '', '455', 'S', '', '',$processo['entid'],'Tipo de Processo');
	                ?>
	            </td>
	        </tr>
	        
	        <?php responsaveisProcesso($prcid)?>
	        
	        <tr style="height:20px;"></tr>

			<tr>
	            <td class="subtituloDireita" colspan="2">Vigência:</td>
	            <td>
	            	<?php 
	            	$vigencia = 't';
	            	$vpajustificativa = '';
		            $vpadtrevogacao = '';
		            $vpadatanao = '';
	            	// dbg($PenalidadeAplicada->vpaid != null,d);
	            	if( $PenalidadeAplicada->vpaid != null ){
		            	$sql = "
		            		select 
		            			vpavigencia,
		            			vpajustificativa,
		            			vpadtrevogacao,
		            			TO_CHAR(vpadatanao,'DD/MM/YYYY') as vpadatanao
		            		from cproc.vigenciapenaplicada
		            		where vpaid = {$PenalidadeAplicada->vpaid}
		            	";
		            	// ver($sql,d);
		            	$vigenciaCarregada = $db->carregar( $sql  );
		            	// ver($vigenciaCarregada,d);
		            	$vigencia = $vigenciaCarregada[0]['vpavigencia'];
		            	$vpajustificativa = $vigenciaCarregada[0]['vpajustificativa'];
		            	$vpadtrevogacao = $vigenciaCarregada[0]['vpadtrevogacao'];
		            	$vpadatanao = $vigenciaCarregada[0]['vpadatanao'];
		            }
	            	?>
	                <input type="radio" name="vpavigencia_campo" value="1" <?=($vigencia && $vigencia == 't')?'checked':''?> <?=($habil=='S')?'':'disabled' ?> />Sim
	                &nbsp;&nbsp;&nbsp;
	                <input type="radio" name="vpavigencia_campo" value="0" <?=($vigencia && $vigencia == 'f')?'checked':''?> <?=($_GET['papid'] || $habil=='S')?'':'disabled'?> />Não
	            </td>
	        </tr>
	        <tr id="linhaVigenciaJustificativa" style="<?=($vigencia=='t')?'display:none;':'display:;'?>">
	        	<td class="subtituloDireita" colspan="2">Justificativa da não vigência:</td>
	            <td>
	            	<textarea style="width:300px;height:150px" <?=($habil=='S')?'':'disabled' ?> maxlength="400" name="vpajustificativa" id="vpajustificativa"><?=$vpajustificativa?></textarea>
	            </td>
	        </tr>
	        <tr id="linhaVigenciaDataRevogacao" style="<?=($vigencia=='t')?'display:none;':'display:;'?>">
	        	<td class="subtituloDireita" colspan="2">Data Revogação:</td>
	            <td>
	            	<?php
	            		echo campo_data2('vpadtrevogacao','N',$habil,'Data Revogação','S','','', $vpadtrevogacao);
	            	?>
	            </td>
	        </tr>
	        <tr id="linhaVigenciaDataNao" style="<?=($vigencia=='t')?'display:none;':'display:;'?>">
	        	<td class="subtituloDireita" colspan="2">Data Não:</td>
	            <td>
	            	<?=$vpadatanao?>
	            </td>
	        </tr>

	        <tr style="height:20px;"></tr>

	        <tr>
                    <td class="subtituloDireita" colspan="2">Objeto Penalidade aplicada</td>
                    <td id="td_municipio">
                        <?PHP 
                            $sql = "
                                SELECT  opaid AS codigo, 
                                        opadsc AS descricao 
                                FROM cproc.objpenaplicada 

                                WHERE opastatus = 'A' 

                            "; 
                            $db->monta_combo('opaid', $sql, $habil, 'Selecione...', '', '', '', '455', 'N', '', '',$PenalidadeAplicada->opaid); 
                        ?>
                    </td>
                </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Data Aplicação:</td>
	            <td>
	                <?php 
	                	$papdataaplicacao = ($PenalidadeAplicada->papdataaplicacao)?$PenalidadeAplicada->papdataaplicacao:'';
	                	echo campo_data2('papdataaplicacao_campo', 'S', $habil, 'Data Aplicação', 'S', '', '', $papdataaplicacao, '', '', 'papdataaplicacao_campo'); 
	                ?>
	            </td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Observações:</td>
	            <td>
	                <textarea style="width:300px;height:150px" <?=($habil=='S')?'':'disabled' ?> maxlength="400" name="papobservacoes_campo" id="papobservacoes_campo"><?=($PenalidadeAplicada->papobservacoes)?$PenalidadeAplicada->papobservacoes:'';?></textarea>
	            </td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Link DOU:</td>
	            <td>
	                <?php
	                	$paplinkdou = ($PenalidadeAplicada->paplinkdou)?$PenalidadeAplicada->paplinkdou:'';
	                    echo campo_texto('paplinkdou_campo', 'N', 'S', 'Link DOU', '51', '100', '', '', '', '', '', 'id="paplinkdou_campo"', '', $paplinkdou, '');
	                ?>
	            </td>
	        </tr>
	        <tr>
	            <td class="SubTituloCentro" colspan="3">
	            	<?php if($habil=='S'){ ?>
		                <input type="button" name="salvarProcesso" id="salvarProcesso" value="<?=( $_GET['papid'] )?'Salvar':'Cadastrar';?>" onclick="salvar();"/>
		                &nbsp;&nbsp;&nbsp;
		                <input name="reset" type="reset" value="Limpar campos"/>
		                <?php if( $_GET['papid'] ){ ?>
		                	&nbsp;&nbsp;&nbsp;
		                	<input name="novo" type="button" value="Cadastrar Nova Penalidade Aplicada" onclick="javascript:window.location.href='cproc.php?modulo=principal/penalidadeaplicada&acao=A'"/>
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
	            <td class="subtituloDireita" colspan="2">Vigência:</td>
	            <td> 
	            	<select name="vpavigencia_filtro" id="vpavigencia_filtro">
	            		<option value=""></option>
	            		<option value="t" <?=($filtros['vpavigencia'] && $filtros['vpavigencia']=='t')?'selected':''?> >Sim</option>
	            		<option value="f" <?=($filtros['vpavigencia'] && $filtros['vpavigencia']=='f')?'selected':''?> >Não</option>
	            	</select>
	            </td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Data Aplicação:</td>
	            <td> 
	            	<?php 
	                	$papdataaplicacao_filtro = ($filtros['papdataaplicacao'])?$filtros['papdataaplicacao']:'';
	                	echo campo_data2('papdataaplicacao_filtro', 'S', 'S', 'Data Aplicação', 'S', '', '', $papdataaplicacao_filtro, '', '', 'papdataaplicacao_filtro'); 
	                ?>
	           	</td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Link DOU:</td>
	            <td> <input name="paplinkdou_filtro" id="paplinkdou_filtro" value="<?=@$filtros['paplinkdou']?>" /> </td>
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
		$cabecalho = array("Ação","Vigência","Data Aplicação","Link Diário Oficial");
		$alinhamento = array('left','left','left','left','left');
		$larguras = array('10%','5%','20%','20%','20%','20%');
		$filtros['prcid'] = $processo['prcid'];
		// ver($PenalidadeAplicada->montaListaQuery( $filtros ),d);
		$db->monta_lista($PenalidadeAplicada->montaListaQuery( $filtros ),$cabecalho,30,5,'N','','N','listaPenalidadeAplicada', $larguras,$alinhamento);
		?>
	</div>

</div>

<script>

function salvar(){

	if (jQuery('[name="opaid"]').val() == ''){
		alert('Informe o Objeto Penalidade aplicada.');
		return;
	}
	
	if (document.getElementById('papdataaplicacao_campo').value == ''){
		alert('Informe a Data Aplicação.');
		return;
	}
	
	document.forms.formPenalidadeAplicada.submit();
}

function editar( chave ){
	window.location.href = "cproc.php?modulo=principal/penalidadeaplicada&acao=A&papid="+chave;
}

function excluir( chave ){
	if( confirm('Deseja apagar esse item?') ){
		window.location.href = "cproc.php?modulo=principal/penalidadeaplicada&acao=A&excluir=1&papid="+chave;
	}
}


jQuery('document').ready(function(){

	// observer para o campo vigencia
	jQuery('[name="vpavigencia_campo"]').click(function(){

		if( jQuery('[name="vpavigencia_campo"]:checked').val() == 0 ){ // não

			jQuery('#linhaVigenciaJustificativa').css('display','');
			jQuery('#linhaVigenciaDataRevogacao').css('display','');
			jQuery('#linhaVigenciaDataNao').css('display','');
		}else{// sim
			
			jQuery('#linhaVigenciaJustificativa').css('display','none');
			jQuery('#linhaVigenciaDataRevogacao').css('display','none');
			jQuery('#linhaVigenciaDataNao').css('display','none');
		}
	});
});

</script>