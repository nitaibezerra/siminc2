<?php

if( possui_perfil(array(PFLCOD_CALL_CENTER)) ){
    $desabCheck = TRUE;
}

switch ($_REQUEST['ajax']){
	case 'municipio':
		header('content-type: text/html; charset=ISO-8859-1');

		$municipio = new Municipio();
		echo $db->monta_combo("muncod", $municipio->listaCombo( array('estuf' => $_POST['estuf']) ), 'S', 'Selecione...', '', '', '', 200, 'N', 'muncod');
		exit;		
}

?>
<html>
	<head>
		<title>Cadastro de Solicitante</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
		
		<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
		<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
		<script type="text/javascript">

                    $(document).ready(function(){
                        //se possui perfil call center - desabilita edição 
                        var desabCheck = "<?php echo $desabCheck;?>";
                        if(desabCheck == true){
                            $("input:checkbox").attr("disabled", true);
                        }
                        $('.pesquisar').click(function(){
                            $('#formListaObra').submit();
                        });
			<?php
                            if ( $abreBuscaAvancada ){
                                echo "exibeBuscaAvancada( " . ($abreBuscaAvancada == 't' ? 'true' : 'false') . " )";	
                            }
			?>
                        window.opener.$('[name*=obrid]').each(function (i, val){
                            $('#obrid_' + this.value).attr('checked', true);
                        });
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
												  url  		 : url,
												  type 		 : 'post',
												  data 		 : {ajax  : 'municipio', 
												  		  	    estuf : estuf},
												  dataType   : "html",
												  async		 : false,
												  beforeSend : function (){
												  	divCarregando();
													td.find('select option:first').attr('selected', true);
												  },
												  error 	 : function (){
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
			
			function vincularObra( objObra ){
				var obrid  = objObra.value;
			
				if ( $(objObra).attr('checked') != 'checked' ){
					window.opener.jQuery('#table_obra_' + obrid).remove();
					return true;
				}
			
				var obrdsc = $('#obr_dsc_' + obrid).val();
				var empdsc = $('#obr_emp_' + obrid).val();
				
				var itemObra = window.opener.jQuery('#vinculo_obra_modelo').clone().attr('id', 'table_obra_' + obrid).show();
				
//				itemObra.find('#txt_empreendimento').html( empdsc );
				itemObra.find('#txt_obra').html( obrid + ' - ' + obrdsc );
				itemObra.find('#obrid').val( obrid );
				
				itemObra.find('#ocrdtordemservico').attr('id', 'ocrdtordemservico_' + obrid);
				itemObra.find('tr:eq(1) td img').attr('onclick', "displayCalendar(document.getElementById('ocrdtordemservico_" + obrid + "'),'dd/mm/yyyy',this)");
																		   
				itemObra.find('#ocrdtinicioexecucao').val( window.opener.jQuery('#crtdtassinatura').val() )
													 .attr('id', 'ocrdtinicioexecucao_' + obrid);
				itemObra.find('tr:eq(2) td img').attr('onclick', "displayCalendar(document.getElementById('ocrdtinicioexecucao_" + obrid + "'),'dd/mm/yyyy',this)");
				
				itemObra.find('#ocrprazoexecucao').val( window.opener.jQuery('#crtprazovigencia').val() );
				itemObra.find('[id*=ocrdtterminoexecucao]').val( window.opener.jQuery('[id*=crtdttermino]').val() ).attr('id', 'ocrdtterminoexecucao_' + obrid);
				
				window.opener.jQuery('#td_obra_contrato').append( itemObra );
				
				var totalObras = window.opener.jQuery('[id*=table_obra_]').length - 1;
				itemObra.find('#arquivoos').attr('name', 'arquivoos_'+totalObras);
				itemObra.find('#arquivocusto').attr('name', 'arquivocusto_'+totalObras);
			}
		//-->
		</script>
<body marginheight="0" marginwidth="0">
<form method="post" name="formListaObra" id="formListaObra">
	<input type="hidden" name="op" id="op" value="">
	<input type="hidden" name="empid" id="empid" value="">
	<input type="hidden" name="abreBuscaAvancada" id="abreBuscaAvancada" value="">
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="15%">Nome da Obra / ID:</td>
			<td>
				<?=campo_texto('obrbuscatexto','N','S','',50,100,'','', '', '', '', 'id="obrbuscatexto"');?>
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
						<td class="SubTituloDireita" style="width: 190px;">Tipo de Ensino:</td>
						<td>
							<?php 
							$orgao = new Orgao();
							$db->monta_combo( "orgid", $orgao->listaCombo(), "S", "Todos", "", "", "", 200, "N", "orgid" );
							?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" style="width: 190px;">Empreendimento:</td>
						<td>
							<?php 
							$empreendimento = new Empreendimento();
							$db->monta_combo( "tobid", $empreendimento->listaCombo(), "S", "Todos", "", "", "", 200, "N", "tobid" );
							?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" style="width: 190px;">Tipo de Obra:</td>
						<td>
							<?php 
							$tipoObra = new TipoObra();
							$db->monta_combo( "tobid", $tipoObra->listaCombo(), "S", "Todos", "", "", "", 200, "N", "tobid" );
							?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" style="width: 190px;">Classificação da Obra:</td>
						<td>
							<?php 
							$classificacaoObra = new ClassificacaoObra();				
							$db->monta_combo( "cloid", $classificacaoObra->listaCombo(), "S", "Todos", "", "", "", 200, "N", "cloid" );
							?>
						</td>
					</tr>
				<?php 
				if ( $_SESSION['obras2']['orgid'] != ORGID_EDUCACAO_PROFISSIONAL ):
				?>
					<tr>
						<td class="SubTituloDireita" style="width: 190px;">Tipologia da Obra:</td>
						<td>
							<?php 
								$tipologiaObra = new TipologiaObra();							
								$db->monta_combo( "tpoid", $tipologiaObra->listaCombo( array("orgid" => $_SESSION['obras2']['orgid']) ), "S", "Todas", "", "", "", 200, "N", "tpoid" );
							?>
						</td>
					</tr>
				<?php
				endif;
				?>
					<tr>
						<td class="SubTituloDireita" style="width: 190px;">Programa:</td>
						<td>
							<?php 
							$programa = new ProgramaFonte();				
							$db->monta_combo( "prfid", $programa->listaCombo( array("orgid" => $_SESSION['obras2']['orgid']) ), "S", "Todos", "", "", "", 200, "N", "prfid" );
							?>
						</td>
					</tr>
					
			<?php if($_SESSION['obras2']['orgid'] != ORGID_EDUCACAO_SUPERIOR && $_SESSION['obras2']['orgid'] != ORGID_EDUCACAO_PROFISSIONAL): ?>
				<tr>
					<td class="SubTituloDireita" style="width: 190px;">Modalidade de Ensino:</td>
					<td>
						<?php 
						$modalidade = new ModalidadeEnsino();
						$db->monta_combo( "moeid", $modalidade->listaCombo(), "S", "Todos", "", "", "", 200, "N", "moeid" );
						?>
					</td>
				</tr>
			<?php endif; ?>
			
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
				<tr>
					<td class="SubTituloDireita" style="width: 190px;">Valor da Obra:</td>
					<td>
						De:&nbsp;
						<?php 
						echo campo_texto( 'obrvalorprevisto_menor', 'N', 'S', '', 11, 30, '[###.]###,##', '', 'right', '', 0, '');
						?>
						Até:&nbsp;
						<?php 
						echo campo_texto( 'obrvalorprevisto_maior', 'N', 'S', '', 11, 30, '[###.]###,##', '', 'right', '', 0, '');
						?>
					</td>
				</tr>
					
					
				</table>			
			</td>
		</tr>
		<tr>
			<td style="background-color:#DCDCDC" width="15%" colspan="2" align="center">
				<input type="button" name="pesquisar" class="pesquisar" value="Pesquisar"/>
			</td>
		</tr>
	</table>
</form>


<?php
$obras 		= new Obras();
$dados 		= $_POST;
$dados['orgid'] = ($_POST['orgid'] ? $_POST['orgid'] : '');
$dados['crtid'] = ($_GET['crtid'] ? $_GET['crtid'] : '');
$dados['licid'] = ($_GET['licid'] ? $_GET['licid'] : '');
$sql   		= $obras->listaSql( $dados, 'vinculoContrato' );

$cabecalho = array("Ação","Obra", "Empreendimento", "Classificação da Obra", "Tipo da Obra", "Valor Previsto");
$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2, "formulario");
?>
</body>
</html>
