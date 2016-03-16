<?php 
set_time_limit(0);
ini_set("memory_limit","256M");

if($_POST['requisicaotramit'] == 'tramitar'){
	
	global $db;
	
	require_once APPRAIZ . 'includes/workflow.php';
	if($_POST['docid']){
		foreach( $_POST['docid'] as $prcid => $docid){
			//tramitação fixa, conforme solicitado por analista Wescley
			$alterado = wf_alterarEstado( $docid, 1418, 'Finalizado', array());
			if($alterado){
		            $db->sucesso('principal/tramitaEscolasLote&acao=A','');
			}else{
		            echo "<script>alert('Erro ao tramitar.');window.history.back();</script>";
			}
		}
	}else{
		echo "<script>alert('Obrigatório escolher uma escola para tramitar.');window.history.back();</script>";
	}

}
?>

<?php 
include_once APPRAIZ . 'includes/workflow.php';
include APPRAIZ . 'includes/cabecalho.inc';
echo '<br/>';
monta_titulo( 'Finaliza Escolas em lote', '' );
?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
				<form action="" method="POST" id="formulario" name="formulario" class="formTramitaEscola">
					<input type="hidden" name="pesquisa" value="1" />
					<input type="hidden" name="acao" value="<?= $_REQUEST['acao'] ?>"/>
					<div style="float: left;">
						<table width="100%" border="0" cellpadding="4" cellspacing="0">
							<tr>
								<td valign="bottom">
									Escola:
									<br/>
									<?php $escola = simec_htmlentities( $_REQUEST['escola'] ); ?>
									<?= campo_texto( 'escola', 'N', 'S', '', 50, 255, '', '' ); ?>
								</td>
								<td valign="bottom">
									Código Escola:
									<br/>
									<?php $entcodent = simec_htmlentities( $_REQUEST['entcodent'] ); ?>
									<?= campo_texto( 'entcodent', 'N', 'S', '', 10, 10, '############', '' ); ?>
								</td>	
							</tr>
							<tr>
     							<td valign="bottom">
									Avaliação
									<br/>
									<?php
									$arrayAvaliacao = array(
														array('codigo' => 'a', 'descricao' => 'A'), 
														array('codigo' => 'b', 'descricao' => 'B'), 
														array('codigo' => 'c', 'descricao' => 'C')																		
														);

									$avaliacao = $_REQUEST['avaliacao'];
									
									$db->monta_combo( "avaliacao", $arrayAvaliacao, 'S', 'Selecione...', '', '' );
									?>
								</td>
                                <td valign="bottom">
                                    Avaliação por pontos
                                    <br/>
                                        <?php
                                        $arrayAvaliacao_pontos = array(
                                            array('codigo' => '0', 'descricao' => '0'),
                                            array('codigo' => '1', 'descricao' => '1'),
                                            array('codigo' => '2', 'descricao' => '2'),
                                            array('codigo' => '3', 'descricao' => '3'),
                                            array('codigo' => '4', 'descricao' => '4')
                                        );

                                        $avaliacao_pontos = $_REQUEST['avaliacao_pontos'];

                                        $db->monta_combo("avaliacao_pontos", $arrayAvaliacao_pontos, 'S', 'Selecione...', '', '');
                                        ?>
                                </td>
                            </tr>							
							<tr>
								<td valign="middle">
                                    Maioria PBF?
                                    <input type="checkbox" name="escolasPBF" value="1" <?php echo ($_REQUEST['escolasPBF'] == 1) ? 'checked' : '' ?>/>Sim
                                </td>								
								<td valign="bottom">
									Estado
									<br/>
									<?php
									if($_REQUEST['estuf'])
										$estuf = $_REQUEST['estuf'];
									elseif( $_SESSION['maiseducacao']['filtro']['estuf'] )
										$estuf = $_SESSION['maiseducacao']['filtro']['estuf'];
									
									$sql = "select
											 e.estuf as codigo, e.estdescricao as descricao 
											from
											 territorios.estado e 
											order by
											 e.estdescricao asc";
									$db->monta_combo( "estuf", $sql, 'S', 'Selecione...', 'filtraTipo', '' );
									?>
								</td>
								<td valign="bottom" id="municipio" style="visibility:<?= ($_REQUEST['estuf'] || $_SESSION['maiseducacao']['filtro']['estuf']) ? 'visible' : 'hidden'  ?>;">
									Município
									<br/>
									<?php
// 									if ($_REQUEST['estuf'] || $_SESSION['maiseducacao']['filtro']['estuf'])
// 									{
										if($_REQUEST['muncod'])
											$muncod = $_REQUEST['muncod'];
										elseif( $_SESSION['maiseducacao']['filtro']['muncod'] )
											$muncod = $_SESSION['maiseducacao']['filtro']['muncod'];
										
										$sql = "select
												 muncod as codigo, 
												 mundescricao as descricao 
												from
												 territorios.municipio
												where
												 estuf = '".$estuf."' 
												order by
												 mundescricao asc";
										$db->monta_combo( "muncod", $sql, 'S', 'Selecione...', '', '' );
// 									}
									?>	
								</td>
							</tr>
						</table>
						<br/>
						<div style="float: left;">
							<input type="hidden" name="requisicao" id="requisicao" value="">
							<input type="button" name="" value="Pesquisar" onclick="return validaForm();"/>
							<input type="button" name="" onclick="limpaCampos()" value="Limpar"/>
							<!--  <input type='button' id='s' value='Finalizar' style='width:120px;' onclick='finalizar();'>-->
							<input type="button" name="tramitar2" value="Finalizar" onclick="javascript: tramita();">	
						</div>
					</div>		
				</form>
			</td>
		</tr>
	</tbody>
</table>
<? listaTramitaEscolas(); ?>
<div id="lista" ></div>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">


$(document).ready(function() {
	//removendo onclick que executa ordenar coluna, da coluna checked.
	$('.title:first-child').attr("onclick","");

});

d = document;
/*
* Faz requisição via ajax
* Filtra o municipio, atravéz do parametro passado 'estuf'
*/
function filtraTipo(estuf) {
	td     = d.getElementById('municipio');
	select = d.getElementsByName('muncod')[0];
	
	/*
	* se estuf vazio
	* esconde o td do municipio e retorna
	*/
	if (!estuf){
		td.style.visibility = 'hidden';
		return;
	}

	// Desabilita o <select>, caso exista, até que o resultado da pesquisa seja carregado
	if (select){
		select.disabled = true;
		select.options[0].text = 'Aguarde...';
		select.options[0].selected = true;
	}	
	
	// Faz uma requisição ajax, passando o parametro 'ordid', via POST
	var req = new Ajax.Request('minc.php?modulo=principal/lista&acao=A', {
							        method:     'post',
							        parameters: '&ajaxestuf=' + estuf,
							        onComplete: function (res)
							        {			
							        	var inner = 'Município<br/>';
										td.innerHTML = inner+res.responseText;
										td.style.visibility = 'visible';
							        }
							  });
    }	

function tramita(){
	if(jQuery("input[@name='docid[]']:checked").length > 0)
	{
 			$('#formtramita').submit();
	}
	else
		alert("Nenhum registro selecionado.");
}
                   
function validaForm()
{
  window.document.getElementById('requisicao').value = '';
  document.formulario.submit();
}

//marcar e desmarcar checked das escolas
function marcaTodos()
{
	$('input[type=checkbox]').each( function(){ 
		if( $(this).is(':checked') ){
	   		$(this).attr('checked', false);
		} else {
	   		$(this).attr('checked', true);
		}
	} );
}

function limpaCampos(){
	$("#formulario input:text").val("");
	$('select option').attr("selected", "");
	$('input:checked[name="escolasPBF"]').attr("checked",false);
}
//function finalizar(){
//	if(jQuery("input[@name='docid[]']:checked").length > 0)
//	{
		//jQuery("input[type=checkbox]", this).each(function() { 
//		$('input[type=checkbox]:checked').each(function(i,value,n){
//             var value = jQuery(this).val();
// 			alert($(value).val());
			
//			wf_alterarEstado( docid, 1418, 'Finalizado', array() );
//		});
		
//	}
//	else
//alert("Nenhum registro selecionado.");
//}



</script>
