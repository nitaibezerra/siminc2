<?php
$_SESSION['obras2']['orgid'] = 3; //$_REQUEST['orgid'] ? $_REQUEST['orgid'] : $_SESSION['obras2']['orgid'];
$_SESSION['obras2']['orgid'] = ($_SESSION['obras2']['orgid'] ? $_SESSION['obras2']['orgid'] : ORGID_EDUCACAO_SUPERIOR);
$orgid 						 = $_SESSION['obras2']['orgid'];

$_SESSION['obras2']['demid'] = '';
$_SESSION['obras2']['obrid'] = '';

function apagar(){

	global $db;
	
	$demanda = new Demanda( $_POST['demid'] );
	if ( $demanda->demid ){
		$demanda->demstatus = 'I';
		$demanda->salvar();
	}
	$db->commit();
	die('<script type="text/javascript">
			alert(\'Operação realizada com sucesso!\');
			location.href=\'?modulo=principal/listaDemandas&orgid=' . $_GET['orgid'] . '&acao=A\';
		 </script>');
		
}

function monta_combo_municipio(){
	
	global $db;
	
	header('content-type: text/html; charset=ISO-8859-1');

	$municipio = new Municipio();
	echo $db->monta_combo("muncod", $municipio->listaCombo( array('estuf' => $_POST['estuf']) ), 'S', 'Selecione...', '', '', '', 200, 'N', 'muncod');
	exit;		
}

if( $_REQUEST['req'] ){
	$_REQUEST['req']();
	die();
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

$arAba = getArAba('listademandas');
echo montarAbasArray($arAba, "?modulo=principal/preobra/listaDemandas&acao=A&orgid=" . $orgid);

monta_titulo( 'Lista de Demandas', 'Filtre as Demandas');

/**
 * EXTRAI OS DADOS DA PESQUISA PARA RECARREGAR O FORM
 */
extract( $_POST );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript">
$(document).ready(function(){

	$('.pesquisar').click(function(){
		$('#formListaDemandas').submit();
	});

	$('.novaDemanda').click(function(){
		window.location.href = 'obras2.php?modulo=principal/preobra/cadDemanda&acao=A';
	});
	
<?php
if ( $abreBuscaAvancada ){
	echo "exibeBuscaAvancada( " . ($abreBuscaAvancada == 't' ? 'true' : 'false') . " )";	
}
?>
});

function exibeBuscaAvancada(visivel){
	if ( visivel == true ){
		$('#tr_busca_avancada').show();
		$('#labelBuscaAvancada').hide();
		$('#abreBuscaAvancada').val( 't' );
	}else{
		$('#tr_busca_avancada').hide();
		$('#labelBuscaAvancada').show();
		$('#abreBuscaAvancada').val( 'f' );
	} 	
}

function carregarMunicipio( estuf ){
	var td	= $('#td_municipio');
	if ( estuf != '' ){
		var url = location.href;
		$.ajax({
				url  		: url,
				type 		: 'post',
				data 		: {req  : 'monta_combo_municipio', 
  		  	    				estuf : estuf},
				dataType   	: "html",
  				async		: false,
  				beforeSend 	: function (){
	  								divCarregando();
									td.find('select option:first').attr('selected', true);
  								},
  				error 	 	: function (){
  									divCarregado();
		  						},
			  	success	 : function ( data ){
							  		td.html( data );
							  		divCarregado();
			  					}
				});	
	}else{
		td.find('select option:first').attr('selected', true);
		td.find('select').attr('selected', true)
						 .attr('disabled', true);
	}			
}

function alterarDem( demid ){
	location.href = '?modulo=principal/preobra/cadDemanda&acao=A&demid=' + demid;
}

function excluirDem( demid ){
	if ( confirm('Deseja apagar esta demanda?') ){
		$('#demid').val(demid);
		$('#req').val('apagar');
		$('#formListaDemandas').submit();
	}
}
</script>
<form method="post" name="formListaDemandas" id="formListaDemandas">
	<input type="hidden" name="req" 	id="req" 	value="">
	<input type="hidden" name="demid" 	id="demid" 	value="">
	<input type="hidden" name="abreBuscaAvancada" id="abreBuscaAvancada" value="">
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="15%">Nome da Demanda / ID:</td>
			<td>
				<?=campo_texto('dembuscatexto','N','S','',70,100,'','', '', '', '', 'id="dembuscatexto"');?>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<a href="javascript:exibeBuscaAvancada( true );" id="labelBuscaAvancada">[Busca avançada]</a>
			</td>
		</tr>
		<tr>
			<td id="tr_busca_avancada" colspan="2" style="display: none;">
				<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
					<tr>
						<th colspan="2">
							Busca Avançada
							<a style="float:right;" onclick="exibeBuscaAvancada( false );">[Fechar]</a>
						</th>
					</tr>
					<tr>
						<td class="SubTituloDireita" style="width: 190px;">Classificação da Demanda:</td>
						<td>
							<?php 
							$demandaClassificacaoObra = new DemandaClassificacaoObra();				
							$db->monta_combo( "dclid", $demandaClassificacaoObra->listaCombo(), "S", "Todos", "", "", "", 200, "N", "dclid" );
							?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" style="width: 190px;">Tipo da Demanda:</td>
						<td>
							<?php 
							$demandaTipoObra = new DemandaTipoObra();
							$db->monta_combo( "dtoid", $demandaTipoObra->listaCombo(), "S", "Todos", "", "", "", 200, "N", "dtoid" );
							?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" style="width: 190px;">Situação do Projeto da Demanda:</td>
						<td>
							<?php 
							$demandaSituacaoProjeto = new DemandaSituacaoProjeto();
							$db->monta_combo( "dspid", $demandaSituacaoProjeto->listaCombo(), "S", "Todos", "", "", "", 200, "N", "dspid" );
							?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" style="width: 190px;">Área da Demanda:</td>
						<td>
							De:&nbsp;
							<?php 
							echo campo_texto( 'demarea_menor', 'N', 'S', '', 11, 30, '[###.]###', '', 'right', '', 0, '');
							?>
							Até:&nbsp;
							<?php 
							echo campo_texto( 'demarea_maior', 'N', 'S', '', 11, 30, '[###.]###', '', 'right', '', 0, '');
							?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" style="width: 190px;">Ano da Demanda:</td>
						<td>
							De:&nbsp;
							<?php 
							echo campo_texto( 'demano_menor', 'N', 'S', '', 5, 4, '####', '', 'right', '', 0, '');
							?>
							Até:&nbsp;
							<?php 
							echo campo_texto( 'demano_maior', 'N', 'S', '', 5, 4, '####', '', 'right', '', 0, '');
							?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" style="width: 190px;">Valor da Demanda:</td>
						<td>
							De:&nbsp;
							<?php 
							echo campo_texto( 'demvalortotal_prev_menor', 'N', 'S', '', 11, 30, '[###.]###,##', '', 'right', '', 0, '');
							?>
							Até:&nbsp;
							<?php 
							echo campo_texto( 'demvalortotal_prev_maior', 'N', 'S', '', 11, 30, '[###.]###,##', '', 'right', '', 0, '');
							?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" style="width: 190px;">Quantidade de alunos atendidos:</td>
						<td>
							De:&nbsp;
							<?php 
							echo campo_texto( 'demqtdalunos_menor', 'N', 'S', '', 11, 30, '[###.]###', '', 'right', '', 0, '');
							?>
							Até:&nbsp;
							<?php 
							echo campo_texto( 'demqtdalunos_maior', 'N', 'S', '', 11, 30, '[###.]###', '', 'right', '', 0, '');
							?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">UF:</td>
						<td>
						<?php
						$uf = new Estado();					
						$db->monta_combo("estuf", $uf->listaCombo(), 'S','Selecione...','carregarMunicipio', '', '',200,'N','estuf'); 
						?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Município:</td>
						<td id="td_municipio">
						<?php 
						if ($estuf){
							$municipio = new Municipio();
							$dado 	   = $municipio->listaCombo( array('estuf' => $estuf) );	
							$habMun    = 'S';
						}else{
							$dado   = array();
							$habMun = 'N';
						}		
						$habMun = ($disable == 'N' ? $disable : $habMun);
						echo $db->monta_combo("muncod", $dado, $habMun,'Selecione...','', '', '',200,'N','muncod'); 
						?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Esfera:</td>
						<td>
							<?php 
							$sql = Array(Array('codigo'=>'E', 'descricao'=>'Estadual'),
										 Array('codigo'=>'M', 'descricao'=>'Municipal'));
							$db->monta_combo('empesfera',$sql, 'S','Selecione...','','','',200,'N', 'empesfera');
							?>
						</td>
					</tr>
				</table>			
			</td>
		</tr>
		<tr>
			<td style="background-color:#DCDCDC" width="15%" colspan="2" align="center">
				<div style="float:left" class="novaDemanda" > 
					<a style="cursor: pointer;"> <img src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0" title="Nova Demanda"> Nova Demanda </a>
				</div>
				<input type="button" name="pesquisar" class="pesquisar" value="Pesquisar"/>
			</td>
		</tr>
	</table>
</form>

<?php
$demanda = new Demanda();
$sql    = $demanda->listaSql( $_POST );

$cabecalho = array("Ação","Demanda", "Classificação da Demanda", "Tipo da Demanda", "Valor Previsto");
$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2, "formulario");
?>
