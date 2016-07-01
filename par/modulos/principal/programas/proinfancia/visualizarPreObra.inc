<?php

function form_questionario(){

	include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
	include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
	
	$_SESSION['par']['preid'] = $preid = $_REQUEST['preid'];
	
	$PreObra = new PreObra($preid);
	
	$qrpid = pegaQrpidPAC( $preid, 43 );
	
	montaTituloPreObra('QUESTIONÁRIO',$PreObra->predescricao); 
?>
	<table bgcolor="#f5f5f5" align="center" class="tabela" >
		<tr>
			<td>
			<fieldset style="width: 94%; background: #fff;"  >
				<legend>Questionário</legend>
				<?php
					$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'tamDivPx' => 250, 'habilitado' => 'N' ) );
				?>
			</fieldset>
		</tr>
	</table>
<?php 
}

function form_analise_engenharia(){

	include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
	include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
	
	$_SESSION['par']['preid'] = $preid = $_REQUEST['preid'];
	
	$PreObra = new PreObra($preid);
	
	$qrpid = pegaQrpidPAC( $preid, 43 );

	montaTituloPreObra('Análise de Engenharia',$PreObra->predescricao);

	$perfil = pegaArrayPerfil($_SESSION['usucpf']);
	
	$obPreObraControle = new PreObraControle();
	
	$arDados 		= $obPreObraControle->recuperarDadosAnaliseEngenharia($preid);
	$arPreAnalise 	= $obPreObraControle->recuperarDadosPorPreid($preid);
	
	$poaid = ($arPreAnalise['poaid']) ? $arPreAnalise['poaid'] : $id;

	if($poaid){?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td colspan="2" class="" style="text-align:center;">
				<a style="cursor: pointer;" onclick="window.open('http://cidades.ibge.gov.br/xtras/perfil.php?lang=&codmun=<?php echo $muncodIBGE ?>&search=Mafra','IBGE','scrollbars=yes,height=400,width=400,status=no,toolbar=no,menubar=no,location=no');" target="_blank"><img style="border: 1px solid black;" src="../imagens/logo_ibge.png" border="0"></a> 
				<a style="cursor: pointer;" onclick="window.open('http://ide.mec.gov.br/2011/municipios/relatorio/coibge/<?php echo $_SESSION['par']['muncod'] ?>','Indicadores','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');" target="_blank"><img style="border: 1px solid black;" src="../imagens/logo_demograficos.png" border="0"></a>
			</td>
		</tr>
		<tr>
			<td colspan="2" class="subtituloesquerda" style="text-align:center;">IDENTIFICAÇÃO</td>
		</tr>
		<tr>
			<td width="150" class="subtitulodireita">Análise N.º:</td>
			<td>
				<?php echo $poaid ?>
			</td>
		</tr>
		<tr>
			<td width="150" class="subtitulodireita">N.º de identificação:</td>
			<td>
				<?php echo $arDados['preid'] ?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Interessado:</td>
			<td>
				<?=$arDados['entnome'] ?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Tipo de obra:</td>
			<td>
				<?php echo $arDados['ptodescricao'] ?>
			</td>
		</tr>	
<?php 
	if(in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) || 
		in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || 
		in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) || 
		in_array(PAR_PERFIL_CONSULTA, $perfil) || 
        in_array(PAR_PERFIL_CONSULTA_MUNICIPAL, $perfil) ||
        in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $perfil) ||
        in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) ||
		in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil) ||
		in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) ||
		in_array(PAR_PERFIL_PREFEITO, $perfil) ||  
		in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $perfil) ||  
		in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil) ||  
		in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) ||  
		in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil)){

		if( ($esdid == WF_TIPO_EM_ANALISE_FNDE ) && $arPreAnalise['poaindeferido'] == '' || ($esdid == WF_TIPO_EM_ANALISE_VALIDACAO && $arPreAnalise['poaindeferido'] == 'N' )){ ?>
			<tr>
				<td colspan="2" class="subtituloesquerda" style="text-align:center;">CONSIDERAÇÕES INICIAIS</td>
			</tr>
			<tr>
				<td colspan="2" class="" style="text-align:center;">
					<table>
						<tr>
							<td align="center" width="100%">			
								<form action="" method="post" id="formulario" name="formulario">
									Com base nos documentos apresentados, fotos, estudo de demanda, planta de localização, é justificável a construção da obra no local?
									<br/>
									<input type="radio" name="poaindeferido" value="S" <?=$arPreAnalise['poaindeferido'] == 'S' ? 'checked' : '' ?> disabled="disabled"><label>Sim.</label>
									<input type="radio" name="poaindeferido" value="N" <?=$arPreAnalise['poaindeferido'] == 'N' ? 'checked' : '' ?> disabled="disabled"><label>Não.</label>
									<div class="poajustificativa" style="display:<?=($arPreAnalise['poaindeferido'] == 'S' || $arPreAnalise['poaindeferido'] == '' ) ? 'none' : '' ?>;"  style="text-align:left;">
										<table align="center">
											<tr>
												<td>
													Justificativa:<br>
													<textarea name="poajustificativa" class="text_editor_simple" cols="50" rows="8"><?=$arPreAnalise['poajustificativa']?></textarea>
												</td>
											</tr>
										</table>
									</div>
								</form>
							</td>
							<td valign="top">
							</td>
						</tr>
					</table>
				</td>
			</tr>
		<?php 
		}elseif(
			$arPreAnalise['poaindeferido'] == 'S' 
			|| $esdid == WF_TIPO_EM_ANALISE_REFORMULACAO
			|| $esdid == WF_TIPO_EM_ANALISE_REFORMULACAO_MI_PARA_CONVENCIONAL
			|| $esdid == WF_TIPO_EM_ANALISE_REFORMULACAO_MI_PARA_CONVENCIONAL_RETORNO_DILIGENCIA 
		){
		?>
			<tr>
				<td colspan="2" class="subtituloesquerda" style="text-align:center;">ANÁLISE DE ENGENHARIA</td>
			</tr>
			<tr>
				<td colspan="2" style="text-align:left;">	
				<table width="100%">
					<tr>
						<td>
						<?php
							$obSubacaoControle2 = new SubacaoControle();
								
							if($preid){
								$arDados = $obSubacaoControle2->recuperarPreObra($preid);
							}
	                                                
							if(in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfil)){
	                        	$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'tamDivPx' => 250, 'habilitado' => 'N', 'perid' => $_GET['perid'], 'relatorio' => 'modulo=principal/programas/proinfancia/questionarioImpressao&acao=A' ) ); 
							}else{
	                        	$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'tamDivPx' => 250, 'habilitado' => 'N', 'perid' => $_GET['perid'] ) );
							}
						?>
					</td>
					<td valign="top" id="td_barra_navegacao">
						<?php 
							if(in_array(PAR_PERFIL_CONSULTA_MUNICIPAL, $perfil) || in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $perfil)){ ?>
						<table style="border: 2px solid rgb(201, 201, 201); background-color: rgb(245, 245, 245); width: 80px;" cellpadding="3" cellspacing="0" border="0">
							<tr style="background-color: rgb(201, 201, 201); text-align: center;">
								<td style="font-size: 7pt; text-align: center;">
									<span title="estado atual">
										<b>Estado atual</b>
									</span>
								</td>
							</tr>
							<tr style="text-align: center;">
								<td style="font-size: 7pt; text-align: center;">
									<?php echo $estadoAtual['esddsc']; ?>
								</td>
							</tr>
						</table>
						<br />
						<br />
						<table style="border: 2px solid rgb(201, 201, 201); background-color: rgb(245, 245, 245); width: 80px;" cellpadding="3" cellspacing="0" border="0">
							<tr style="background-color: rgb(201, 201, 201); text-align: center;">
								<td style="font-size: 7pt; text-align: center;">
									<span title="estado atual">
										<b>análise</b>
									</span>
								</td>
							</tr>
							<tr style="text-align: center;">
								<td style="font-size: 7pt; text-align: center;">
									<a id="<?php echo $preid."_".$muncod."_".$qrpid."_49" ?>" href="javascript:void(0)" class="mostra">Imprimir análise</a>
								</td>
							</tr>
						</table>
                    <?php 
	                    }; 
	                    if(in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) || in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfil)){ ?>
						<table style="border: 2px solid rgb(201, 201, 201); background-color: rgb(245, 245, 245); width: 80px;" cellpadding="3" cellspacing="0" border="0">
							<tr style="background-color: rgb(201, 201, 201); text-align: center;">
								<td style="font-size: 7pt; text-align: center;">
									<span title="estado atual">
										<b>análise</b>
									</span>
								</td>
							</tr>
							<tr style="text-align: center;">
								<td style="font-size: 7pt; text-align: center;">
									<!-- a onclick="javascript: arvore.s(<?php echo $qrpid ?>);" href="javascript:imprimeQ(49)" class="node" id="sarvore44">Imprimir questionário</a -->
									<a id="<?php echo $preid."_".$muncod."_".$qrpid."_49" ?>" href="javascript:void(0)" class="mostra">Imprimir análise</a>
								</td>
							</tr>
						</table>
					<?php
						if(  	
							in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) || 
							in_array(PAR_PERFIL_ADMINISTRADOR, $perfil ) || 
							in_array( PAR_PERFIL_COORDENADOR_GERAL, $perfil ) 
							){
 
							if( in_array( $esdid, array(WF_TIPO_VALIDACAO_DILIGENCIA, WF_TIPO_EM_CORRECAO) )  ) {
							?>
						<br><br>
						<table style="border: 2px solid rgb(201, 201, 201); background-color: rgb(245, 245, 245); width: 80px;" cellpadding="3" cellspacing="0" border="0">
							<tbody>
								<tr style="background-color: rgb(201, 201, 201); text-align: center;">
									<td style="font-size: 7pt; text-align: center;">
										<span title="estado atual">
											<b>Diligência</b>
										</span>
									</td>
								</tr>
							<tr style="text-align: center;">
								<td style="font-size: 7pt; text-align: center;">
									<a style="cursor: pointer" alt="Versão para impressão" onclick="informarDia('<?php echo $preid; ?>',  '<?php echo $qrpid; ?>')">
										<?=( in_array( $esdid, Array(WF_TIPO_EM_CORRECAO) ) ? "Ajustar prazo diligência" : "Enviar para diligência" )  ?>
									</a>
								</td>
							</tr>
						</table>
							<?php
							}
						}
					} ?>
					</td>
					</tr>
					<?php if(!empty($consideracoesFinais['cmddsc'])){ ?>
						<tr>
							<td colspan="2" height="20">
								
							</td>
						</tr>
						<tr>
							<td valign="top" class="subtituloesquerda" colspan="2" style="padding:5px;text-align:center;">						
								<b>CONSIDERAÇÕES FINAIS</b>				
							</td>							
						</tr>
						<tr>
							<td colspan="2">
								<?php echo $consideracoesFinais['cmddsc'] ?>
							</td>
						</tr>
					<?php }; ?>
					</table>
				</td>
			</tr>
		<?php 
		}elseif($arPreAnalise['poaindeferido'] == 'N'){ ?>
			<tr>
				<td colspan="2" class="subtituloesquerda" style="text-align:center;">OBRA INDEFERIDA</td>
			</tr>
			<tr>
				<td valign="top" class="subtitulodireita">
					Justificativa:
				</td>
				<td>
					<table width="100%">
					<tr>
						<td valign="top" align="left">
						<?php echo $arPreAnalise['poajustificativa'] ?>
						</td>
						<td>
					</td>
					<td valign="top" align="right">
					<?php if(in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) || in_array( PAR_PERFIL_COORDENADOR_GERAL, $perfil ) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfil)){ ?>
						<table style="border: 2px solid rgb(201, 201, 201); background-color: rgb(245, 245, 245); width: 80px;" cellpadding="3" cellspacing="0" border="0">
							<tbody>
								<tr style="background-color: rgb(201, 201, 201); text-align: center;">
									<td style="font-size: 7pt; text-align: center;">
										<span title="estado atual">
											<b>análise</b>
										</span>
									</td>
								</tr>
							<tr style="text-align: center;">
								<td style="font-size: 7pt; text-align: center;">
									<a id="<?php echo $preid."_".$muncod."_".$qrpid."_49" ?>" href="javascript:void(0)" class="mostra">Imprimir questionário</a>
								</td>
							</tr>
						</table>
					<?php
						if( ( in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) || in_array( PAR_PERFIL_COORDENADOR_GERAL, $perfil ) ) && ( in_array( $esdid, array(WF_TIPO_VALIDACAO_DILIGENCIA) ) ) ){
							?>
						<br><br>
						<table style="border: 2px solid rgb(201, 201, 201); background-color: rgb(245, 245, 245); width: 80px;" cellpadding="3" cellspacing="0" border="0">
							<tbody>
								<tr style="background-color: rgb(201, 201, 201); text-align: center;">
									<td style="font-size: 7pt; text-align: center;">
										<span title="estado atual">
											<b>Diligência</b>
										</span>
									</td>
								</tr>
							<tr style="text-align: center;">
								<td style="font-size: 7pt; text-align: center;">
									<a style="cursor: pointer" alt="Versão para impressão" onclick="informarDia('<?php echo $preid; ?>',  '<?php echo $qrpid; ?>')">Enviar para diligência</a>
								</td>
							</tr>
						</table>
							<?php
						}
					}; ?>
					</td></tr></table>
				</td>
			</tr>
			<?php 
			if(!empty($consideracoesFinais['cmddsc'])){ ?>
				<tr>
					<td valign="top" class="subtituloesquerda" colspan="2" style="padding:5px;text-align:center;">						
						<b>CONSIDERAÇÕES FINAIS</b>
					</td>
					<td>
						<?php echo $consideracoesFinais['cmddsc'] ?>
					</td>
				</tr>
			<?php 
			}; 
		}elseif($arPreAnalise['poaindeferido'] != 'S' && $arPreAnalise['poaindeferido'] != 'N' && $esdid == WF_TIPO_OBRA_ARQUIVADA){?>
			<tr>
				<td colspan="2" class="subtituloesquerda" style="text-align:center;">SITUAÇÃO</td>
			</tr>
			<tr>
				<td colspan="2" style="text-align:center;">
					A análise desta obra não foi realizada.
				</td>
			</tr>
		<?php 
		} ?>
	</table>
	<?php } ?>
<?php }else{ ?>
	<center><p>Sem análise.</p></center>
<?php } 
}

function detalheObras(){
	
	$endereco = $_REQUEST['endlog'].' - '.$_REQUEST['endnum'].', '.$_REQUEST['endcom'].', '.$_REQUEST['endbai'];
	?>
	<table>
		<tr>
			<td>Nome da Obra:</td>
			<td><?=$_REQUEST['obrdesc'] ?></td>
		</tr>
		<tr>
			<td>Programa/Fonte:</td>
			<td><?=$_REQUEST['plititulo'] ?></td>
		</tr>
		<tr>
			<td>Tipo de Obra:</td>
			<td><?=$_REQUEST['tobadesc'] ?></td>
		</tr>
		<tr>
			<td>Valor da Obra:</td>
			<td><?=number_format($_REQUEST['obrvalorprevisto'],2,',','.'); ?></td>
		</tr>
		<tr>
			<td>Endereço completo:</br>CEP:</td>
			<td><?=$endereco ?></br> <?=$_REQUEST['endcep'] ?></td>
		</tr>
		<tr>
			<td>Situação:</td>
			<td><?=$_REQUEST['situacao'] ?></td>
		</tr>
	</table>
	<?php 
}

function download(){

	require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	
	$obPreObra 	= new PreObraControle();
	$obPreObra->documentoDownloadAnexo($_GET['arqid']);
}

if( $_REQUEST['req'] ){
	ob_clean();	
	$_REQUEST['req']();
	die();
}

$_SESSION['par']['preid'] = $preid = $_REQUEST['preid'];

$PreObra = new PreObra($preid);

$qrpid = pegaQrpidPAC( $preid, 43 );

function montaTituloPreObra( $titulo, $subtitulo ){
?>
<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">
	<tbody>
		<tr>
			<td width="100%" align="center">
				<label class="TituloTela" style="color:#000000;"><?=$titulo ?></label>
			</td>
		</tr>
		<tr>	
			<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#dcdcdc', gradientType='1')">
				<?=$subtitulo ?>
			</td>
		</tr>
	</tbody>
</table>
<?php 
}

// ver($PreObra);
?>
<link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css"/>

<script type="text/javascript" src="../includes/funcoes.js" ></script>
<script language="javascript" type="text/javascript" src="../../library/jquery/jquery-1.11.1.min.js"></script>

<script type="text/javascript" src="../../includes/remedial.js"></script>
<script type="text/javascript" src="../../includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/superTitle.css"/>

<center>
	<div id="aguarde_" style="display: none;position:absolute;color:#000033;top:50%;left:35%; width:300;font-size:12px;z-index:0;">
		<br><img src="../imagens/carregando.gif" border="0" align="middle"><br>Carregando...<br>
	</div>
</center>

<script>
$(document).ready(function(){
	
	jQuery(".anexo").click(function(){
		return window.open(window.location.href+'&req=download&arqid='+this.id, 
						   'anexo', 
						   "height=300,width=450,scrollbars=yes,top=50,left=200" );
	});
	
	var param = 'requisicao=montaArvoreAberta&db=1&ptoid=<?=$PreObra->ptoid ?>';
	
	<?php if( $PreObra->pretipofundacao && $PreObra->ptoid == OBRA_TIPO_B): ?>
	param += "&tipoFundacao=<?=$PreObra->pretipofundacao; ?>";
	<?php endif; ?>
	
	var cor 					= "#f0f0f0";
	var total_valor_unitario 	= 0;
	var total_valor 			= 0;
	var arrItens 				= new Array();
	var num 					= 0;

	$.ajax({
   		type	: "POST",
   		url		: "ajax.php",
   		data	: param,
   		async   : false,
   		dataType: 'json',
   		success	: function(data){	

			$('#aguarde_').show();	
			
			var idNivel = new Array();
			var totalValor = 0;
			var totalValorUni = 0;
			var data;

			$.each(data, function(i,item){
				var itcid 					= item.itcid;
				var itcidpai 				= item.itcidpai;
	       		var boFilho 				= item.boFilho;
	       		var img 					= item.img;
	       		var itccodigoitem			= item.itccodigoitem;
	       		var itcdescricao			= item.itcdescricao;
	       		var itccodigoitemcodigo		= item.itccodigoitemcodigo;
	       		var itcquantidade			= item.itcquantidade;
	       		var umdeesc					= item.umdeesc;
	       		var ppovalorunitario		= item.ppovalorunitario;
	       		var ppoid					= item.ppoid;
	       		var ptopreencher			= item.ptopreencher;
				var tamanho     			= itccodigoitemcodigo.length;
				var nivel 	 				= tamanho / 3;
			
				if(umdeesc == null){
					umdeesc = '-';
				}
				total_valor_unitario = ( (total_valor_unitario * 1) + (ppovalorunitario * 1) );
				total_valor = ( (total_valor * 1) + (ppovalorunitario * itcquantidade) );

				if(itcid){
					arrItens[num] = new Object();
					arrItens[num].itcid = itcid;
					arrItens[num].valor_unitario = ppovalorunitario;
					arrItens[num].valor = (ppovalorunitario * itcquantidade);
					num++;
				}

				idNivel[nivel] = itcid;
				var id = '';
				// prepara para forma o id das TR
				for (i=1; i <= nivel; i++){
					id += (i == 1 ? idNivel[i] : '_' + idNivel[i]);
				}

				// Identação
	       		var espaco     = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
	       		var espacoTemp = "";

				if(itcidpai){
		       		for (y = 1; y < nivel; y++) {
		            	espacoTemp = espacoTemp + espaco;
		            }
				}
            
	            var seta = "";
	            if(espacoTemp){
	            	seta = "<img src=\"../imagens/seta_filho.gif\">";
	            }

	            if(cor == "#fafafa") {
					cor = "#f0f0f0";
				} else {
					cor = "#fafafa";
				}

				var html = "<tr id=\""+id+"\" style=\"background: "+cor+" \" cor=\""+cor+"\">";
				if( boFilho > 0 ) {
					html += "<td>"+espacoTemp+seta
					+"<a href=\"#\" onclick=\"alteraIcone('"+id+"');\"><img id=\"img_"+id+"\" src=\"../imagens/"+img+"\" border=\"0\"></a> "
					+itccodigoitem+" "+itcdescricao+"</td>";
				} else {
					html += "<td>"+espacoTemp+seta+itccodigoitem+" "
					+itcdescricao+"</td>";
				}
				
			 
				// Valor Unitário = ppovalorunitario
				var stAtivo = '<?=$stAtivo ?>';
				
				if( trim(umdeesc) != '' ) {
					html += "<td>"+mascaraglobal('###.###.###.###,##',parseFloat(ppovalorunitario).toFixed(2))+"</td>";								
				} else {
					html += "<td id=\"valor_unitario_pai_"+itcid+"\"></td>";
				}
			
				// Unidade de Medida
				html += "<td align=\"center\" id=\"umdeesc"+umdeesc+" \">"+umdeesc+"</td>";
				
				// Quantidade = itcquantidade
				// Verifico se são obras MI
				if( boFilho < 1 ) {
					html += "<td class=\"quantidade\" align=\"right\" id=\"quantidade_"+itcid+" \">"+itcquantidade+"</td>";
				} else {
					html += "<td id=\"qtde_pai_"+itcid+"\"></td>";
				}
			
				// Valor e porcentagem
				if( boFilho < 1 ) {
					html += "<td align=\"right\" id=\"valor_"+itcid+"\" class=\"valor\">" + (ppovalorunitario * itcquantidade).toFixed(2).replace(".",",") + "</td>";
					html += "<td align=\"right\" id=\"porcentagem_"+itcid+"\"></td>";
				} else {
					html += "<td id=\"valor_pai_"+itcid+"\" ></td>";
					html += "<td id=\"percent_pai_"+itcid+"\" ></td>";
				}
				html += "</tr>";

				$('#tabela_planilha tr:last').after(html);	
        	
			});

			//alert(totalValor)
			var html2 = "<tr style=\"background: #e0e0e0\" >";
				html2 += "<td><strong>TOTAL:</strong></td>";
				
				html2 += "<td id=\"totalValorUni\" align=\"right\"></td>";
				
				html2 += "<td></td>";
				html2 += "<td></td>";
				html2 += "<td id=\"totalValor\" align=\"right\"><strong>" + parseFloat(total_valor).toFixed(2).replace(".",",") + "</strong></td>";
				html2 += "<td align=\"right\" ><strong>100</strong></td>";
				html2 += "</tr>";
			
			$('#tabela_planilha tr:last').after(html2);

			for (i=0;i<=arrItens.length;i++){
				if(arrItens[i]){
					if(arrItens[i].valor == 0 && total_valor == 0){
						var valor = 0; 
					} else {
						var valor = arrItens[i].valor / total_valor;
					}
					$('#porcentagem_' + arrItens[i].itcid ).html(  parseFloat( valor * 100).toFixed(2).replace(".",",")  );
				}			
			}
		
			$('#aguarde_').hide();
   		}
	});

	$('.botao').click(function(){
		$('.aba').hide();
		$('.active').removeClass('active');
		var div = $(this).attr('div');
		if( div == 'questionario' || div == 'analise_engenharia' ){
			window.location.href = 'par.php?modulo=principal/programas/proinfancia/visualizarPreObra&acao=A&preid=<?=$preid ?>&div='+div;
		}
		$('#'+div).show();
		$(this).addClass('active');
	});
});
</script>

<table align="center" class="Tabela" cellpadding="2" cellspacing="1">
	<tbody>
		<tr>
			<td width="30%" style="text-align: right;" class="SubTituloDireita">UF:</td>
			<td width="70%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
				<?=$PreObra->estufpar ?>
			</td>
		</tr>
    	<tr>
			<td width="30%" style="text-align: right;" class="SubTituloDireita">Município:</td>
			<td width="70%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
				<?=($db->pegaUm( "SELECT mundescricao FROM territorios.municipio WHERE muncod = '{$PreObra->muncodpar}'")); ?>
			</td>
		</tr>
		<tr>
			<td width="30%" style="text-align: right;" class="SubTituloDireita">Valor Empenhado:</td>
			<td width="70%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
				<?=number_format(($db->pegaUm("SELECT saldo FROM par.vm_saldo_empenho_por_obra WHERE preid = $preid")), 2, '.', ',') ?>
			</td>
		</tr>
	</tbody>
</table>

<div class="row notprint">
	<div class="col-md-12">
		<ul class="nav nav-tabs" style="margin-left:1px; margin-bottom:-2px;">
			<li class="botao <?=(!in_array($_REQUEST['div'], Array('questionario', 'analise_engenharia', 'planilha') ) ? 'active' : '' ) ?>" div="dados"><a>Dados do terreno</a></li>
			<li class="botao <?=($_REQUEST['div'] == 'questionario' ? 'active' : '' ) ?>" div="questionario"><a>Relatório de vistoria</a></li>
			<li class="botao" div="fotos"><a>Cadastro de fotos do terreno</a></li>
			<li class="botao <?=($_REQUEST['div'] == 'planilha' ? 'active' : '' ) ?>" div="planilha"><a>Planilha orçamentária</a></li>
			<li class="botao" div="documentos"><a>Documentos anexos</a></li>
			<li class="botao" div="analise"><a>Enviar para análise</a></li>
			<li class="botao <?=($_REQUEST['div'] == 'analise_engenharia' ? 'active' : '' ) ?>" div="analise_engenharia"><a>Análise de Engenharia</a></li>
			<li class="botao" div="listaObras"><a>Obras no Município</a></li>
			<li class="botao" div="documentoFNDE"><a>Documentos FNDE</a></li>
			<li class="botao" div="dadosOrcamentarios"><a>Dados Orçamentários</a></li>
		</ul>
	</div>
</div>

<div id="dados" class="aba" <?=(!in_array($_REQUEST['div'], Array('questionario', 'analise_engenharia', 'planilha') ) ? '' : 'style="display:none"' ) ?>>
	<?php montaTituloPreObra('Dados do terreno',$PreObra->predescricao); ?>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tbody>
			<tr>
				<td class="subtitulodireita">Nome do terreno:</td>
				<td><?=$PreObra->predescricao ?></td>
			</tr>
			<tr>
				<td class="subtitulodireita">Tipo da Obra:</td>
				<td><?=( $PreObra->ptoid != '' ? $db->pegaUm("SELECT ptodescricao FROM obras.pretipoobra WHERE ptoid = {$PreObra->ptoid}") : '') ?></td>
			</tr>
			<tr>
				<td align="left" colspan="2"><strong>Endereço do terreno</strong></td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita" style="width: 25%; white-space: nowrap">
					<label>CEP:</label>
				</td>
				<td><?=formata_cep($PreObra->precep) ?></td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
					<label>Logradouro:</label>
				</td>
				<td><?=($PreObra->prelogradouro) ?></td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
					<label>Número:</label>
				</td>
				<td><?=($PreObra->prenumero) ?></td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
					<label>Complemento:</label>
				</td>
				<td><?=($PreObra->precomplemento) ?></td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
					<label>Ponto de Referência:</label>
				</td>
				<td><?=($PreObra->prereferencia) ?></td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
					<label>Bairro:</label>
				</td>
				<td><?=($PreObra->prebairro) ?></td>
			</tr>
			<tr>
				<td class="subtitulodireita">Estado:</td>
				<td><?=($PreObra->estuf) ?></td>
			</tr>
			<tr>
				<td class="subtitulodireita">Município:<br></td>
				<td>
				<?=($db->pegaUm( "SELECT mundescricao FROM territorios.municipio WHERE muncod = '{$PreObra->muncod}'")); ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Latitude :</td>
				<td>
					<?php $lat = explode('.',$PreObra->prelatitude); ?>
					<span id="_graulatitude1"><?=($lat[0]) ?></span> º
					<span id="_minlatitude1"><?=($lat[1]) ?></span>  '
					<span id="_seglatitude1"><?=($lat[2]) ?></span> "
					<span id="_pololatitude1"><?=($lat[3]) ?></span>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Longitude :</td>
				<td>
					<?php $long = explode('.',$PreObra->prelongitude); ?>
					<span id="_graulongitude1"><?=($long[0]) ?></span>	º
					<span id="_minlongitude1"><?=($long[1]) ?></span>  '
					<span id="_seglongitude1"><?=($long[2]) ?></span> "
					<span id="_pololongitude1"><?=($long[3]) ?></span>
				</td>
			</tr>
		</tbody>
	</table>
</div>

<div id="fotos" class="aba" style="display:none">
	<?php montaTituloPreObra('Cadastro de fotos',$PreObra->predescricao); ?>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td colspan="2" class="SubTituloCentro">
				<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
				<script type="text/javascript" src="../includes/remedial.js"></script>
				<script type="text/javascript" src="../includes/superTitle.js"></script>
				<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
				<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
				<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
				<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
				<script type="text/javascript">
					messageObj = new DHTML_modalMessage();	// We only create one object of this class
					messageObj.setShadowOffset(5);	// Large shadow
					
					function displayMessage(url) {
						messageObj.setSource(url);
						messageObj.setCssClassMessageBox(false);
						messageObj.setSize(690,400);
						messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
						messageObj.display();
					}
					function displayStaticMessage(messageContent,cssClass) {
						messageObj.setHtmlContent(messageContent);
						messageObj.setSize(600,150);
						messageObj.setCssClassMessageBox(cssClass);
						messageObj.setSource(false);	// no html source since we want to use a static message here.
						messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
						messageObj.display();
					}
					function closeMessage() {
						messageObj.close();	
					}
				</script>
				<?
				$sql = "SELECT 
							arqnome, arq.arqid, 
							arq.arqextensao, arq.arqtipo, 
							arq.arqdescricao,							
						 	to_char(arq.arqdata, 'DD/MM/YYYY') as data, 
						 	arc.arqvalidacao
						FROM 
							public.arquivo arq 
						LEFT  JOIN public.arquivo_recuperado arc ON arc.arqid = arq.arqid 
						INNER JOIN obras.preobrafotos pof ON arq.arqid = pof.arqid
						INNER JOIN obras.preobra pre ON pre.preid = pof.preid
						WHERE							
							pre.preid = {$preid} 
							AND	(substring(arqtipo,1,5) = 'image') 
						ORDER BY 
							arq.arqid";
				
				$fotos = ($db->carregar($sql));
				
				$_SESSION['downloadfiles']['pasta'] = array("origem" => "obras","destino" => "obras");				

				if( $fotos ){

					$_SESSION['imgparams'] = Array("filtro" => "cnt.preid={$preid}", 
												   "tabela" => "obras.preobrafotos");
					for( $k=0; $k < count($fotos); $k++ ){
						
						echo "<div style=\"{$alerta}float:left; width:90px; height:140px; text-align:center; margin:2px;\" >
								<img title=\"".$fotos[$k]["arqdescricao"]."\" border='1px' id='".$fotos[$k]["arqid"]."' src='../slideshow/slideshow/verimagem.php?newwidth=64&newheight=48&arqid=".$fotos[$k]["arqid"]."&_sisarquivo=obras' hspace='10' vspace='3' style='position:relative; z-index:5; float:left; width:70px; height:70px;' onmouseover=\"return escape( '". $fotos[$k]["arqdescricao"] ."' );\" onclick='javascript:window.open(\"../slideshow/slideshow/index.php?pagina=". $_REQUEST['pagina'] ."&_sisarquivo=obras&arqid=\"+this.id+\"\",\"imagem\",\"width=850,height=600,resizable=yes\")'/><br>
								" . $fotos[$k]["data"] . " <br/>
								" . $fotos[$k]["acao"] . "
							  </div>";
						
					}
					
				}else {
					echo "Não existem fotos cadastradas";
				}
				?>
			</td>
		</tr>
	</table>
</div>

<div id="planilha" class="aba" <?=($_REQUEST['div'] == 'planilha' ? '' : 'style="display:none"' ) ?>>
	<?php montaTituloPreObra('Planilha Orçamentária',$PreObra->predescricao); ?>
	<table id="tabela_planilha" class="tabela" bgcolor="#f5f5f5" cellpadding="3" align="center">
		<tr style="background-color: #e0e0e0">
			<td style="font-weight:bold; text-align:center; width:20%;">Descriçao do item</td>
			<td style="font-weight:bold; text-align:center; width:10%;">Valor Unitario</td>			
			<td style="font-weight:bold; text-align:center;">Unidade de Medida</td>
			<td style="font-weight:bold; text-align:center; width:30%;">Quantidade</td>
			<td style="font-weight:bold; text-align:center; width:10%;">Valor</td>
			<td style="font-weight:bold; text-align:right; width:10%;">%</td>			
		</tr>
	</table>
</div>

<div id="documentos" class="aba" style="display:none">
	<?php 
		montaTituloPreObra('Documentos Anexos',$PreObra->predescricao);

		$obPreObra 	= new PreObraControle();
		
		$arTipoObraDocumentos = $obPreObra->recuperarTiposObraDocumentosProInfancia($preid);
		$arTipoObraDocumentos = $arTipoObraDocumentos ? $arTipoObraDocumentos : array();
	?>
	<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
		<table width="95%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
			<thead>
				<tr>
					<td valign="top" width="40" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><b>Item</b></td>
					<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><b>Descrição</b></td>
					<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><b>Anexo(s)</b></td>
					<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><b>Observações</b></td>
				</tr>
			</thead>
			<tbody>
				<?php $x = 0; ?>
				<?php foreach($arTipoObraDocumentos as $tipos): ?>
					<?php
					$cor = ($x % 2) ? "#F7F7F7" : "white"; 
					?>
					<tr bgcolor="<?php echo $cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $cor ?>';">
						<td align="center"><?php echo $x+1 //$tipos['codigo'] ?></td>			
						<td><?php echo $tipos['descricao'] ?></td>
						<td align="right" >
							<table width="100%" border="0" cellspacing="0" cellpadding="0">
								<tr>
									<td>
										<?php
										if( ($esdid == WF_TIPO_EM_CORRECAO) && possuiPerfil($arrPerfil) ){
											switch ($tipos['codigo']) {
											    case "3":
											        $trav = in_array(QUESTAO_ESTUDO_DEM,$respSim);
											        $txtAjuda = "Anexar o Estudo de Demanda elaborado, devidamente assinado, justificando e apresentando a necessidade de implantação da obra solicitada, focado na região de abrangência, informando o número de crianças a serem atendidas na faixa etária pleiteada e apresentando as fontes de pesquisa (ex. levantamento da Secretaria de Saúde sobre vacinação infantil).";
											        break;
											    case "1":
											        $trav = in_array(QUESTAO_PLANILHA_LOCALIZACAO,$respSim);
											        $txtAjuda = "Anexar arquivo de imagem da Planta de Localização, destacando o terreno selecionado na malha urbana, indicando o endereço, apresentando seu entorno, os acessos ao lote, as referências naturais e construídas próximas, localizando o terreno proposto dentro do município. ";
											        break;
											    case "4":
											        $trav = in_array(QUESTAO_PLANILHA_SIT,$respSim);
											        $txtAjuda = "Anexar arquivo de imagem da Planta de Situação, indicando as dimensões do terreno e apresentando os lotes e vias limítrofes.";
											        break;
											    case "6":
											        $trav = in_array(QUESTAO_PLANILHA_LOCACAO,$respSim);
											        $txtAjuda = "Anexar arquivo de imagem da Planta de Locação da obra solicitada no terreno proposto. É necessário: 1. Apresentar a Planta Baixa da obra solicitada (disponibilizada no site do FNDE), indicando as dimensões do terreno e as dimensões gerais da edificação, bem como os afastamentos entre esta e os limites do terreno. Cabe ressaltar que o afastamento das edificações em relação aos limites do terreno deverá obedecer às determinações do Plano Diretor local e demais leis vigentes; 2. Locar a entrada das redes de abastecimento de energia, água e esgoto, sempre respeitando as orientações técnicas definidas pelas concessionárias locais. Em caso de utilização de fossa séptica, sumidouro, cisterna ou outro elemento que tenha como função suprir deficiência em redes existentes, tais instrumentos deverão ser também locados; 3. Definição das cotas de nível de cada edificação existente, bem como dos acessos e áreas verdes em relação à via de acesso ou às calçadas limítrofes; 4. Apresentar norte magnético e ventos dominantes; 5. Representar em planta o escoamento de águas pluviais, muros de contenção e qualquer outro tipo de obra que não conste em projeto-padrão, mas que seja imprescindível ao funcionamento adequado da escola.";
											        break;
											    case "5":
											        $trav = in_array(QUESTAO_PLANIALTIMETRICO,$respSim);
											        $txtAjuda = "Encaminhar o Levantamento Planialtimétrico do terreno proposto, apresentando as curvas de nível cotadas a cada metro de desnível.";
											        break;
											    case "9":
											        $trav = in_array(QUESTAO_DOMINIALLIDADE,$respSim);
											        $txtAjuda = "Gerar a declaração padrão de dominialidade, disponibilizada no sistema, preenchendo os dados solicitados; imprimir o documento gerado que deve ser assinado pelo prefeito ou represente legal do município e anexar em campo próprio o arquivo de imagem do documento ou, caso já tenha-se disponível, da Certidão de Matrícula do Imóvel.";
											        break;
											    case "19":
											        $trav = in_array(QUESTAO_DOMINIALLIDADE,$respSim);
											        $txtAjuda = "Gerar a declaração padrão de dominialidade, disponibilizada no sistema, preenchendo os dados solicitados; imprimir o documento gerado que deve ser assinado pelo prefeito ou represente legal do município e anexar em campo próprio o arquivo de imagem do documento ou, caso já tenha-se disponível, da Certidão de Matrícula do Imóvel.";
											        break;
											    case "7":
											        $trav = in_array(QUESTAO_FORNECIMENTO_INFRA,$respSim);
											        $txtAjuda = "Gerar a declaração padrão de responsabilidade pelo fornecimento e manutenção dos serviços de abastecimento de água, energia elétrica, esgotamento sanitário e pela coleta de lixo para o terreno proposto para edificação do objeto pleiteado, além da execução dos serviços de terraplanagem, caso sejam necessários. Preencher os dados solicitados; imprimir o documento gerado que deve ser assinado pelo prefeito ou represente legal do município e anexar em campo próprio o arquivo de imagem do documento.";
											        break;
											    case "8":
											        $trav = in_array(QUESTAO_COMPATIBILIDADE_FUNDACAO,$respSim);
											        $txtAjuda = "Gerar a declaração padrão de adequação da fundação, disponibilizada no sistema, preenchendo os dados solicitados; imprimir o documento gerado que deve ser assinado por profissional competente e anexar em campo próprio o arquivo de imagem do documento. No caso de verificar-se inadequação do solo ao tipo de fundação proposta, o município deverá apresentar novo projeto executivo de fundação contendo, inclusive, a Anotação de Responsabilidade Técnica - ART.";
											        break;
											    case "10":
											        $trav = in_array(QUESTAO_PLANILHA,$respSim);
											        $txtAjuda = "Gerar a Planilha orçamentária padrão preenchida";
											        break;
											}
										}
										$arAnexos = $obPreObra->recuperarDocumentosAnexoPorPodid($tipos['codigo'], $preid);
										$arAnexos = $arAnexos ? $arAnexos : array();
										echo '<table width=100% border="0" cellspacing="0" cellpadding="0">';
										foreach($arAnexos as $anexo){
											echo '<tr><td>
													<a title="Baixar arquivo" href="javascript:void(0)" id="'.$anexo['codigo'].'" class="anexo" style="'.$alerta.'">'
														.delimitador($anexo['descricao'], 20).'.'.$anexo['extensao']
													.'</a></td></tr>';
										}
										echo '</table>';
										?>
									</td>
								</tr>						
							</table>															
						</td>
						<td align="right" width="80">
							<?php
							if( $txtAjuda != '' ){
								$imgAjuda = "<img alt=\"{$txtAjuda}\" title=\"{$txtAjuda}\" src=\"/imagens/ajuda.gif\">";
								echo $imgAjuda; 
							}
							?>
						</td>
					</tr>
					<?php $x++ ?>
				<?php endforeach; ?>
			</tbody>
	</table>
</div>

<div id="analise" class="aba" style="display:none">
	<?php montaTituloPreObra('Análise',$PreObra->predescricao); ?>
	<table class="tabela" align="center">
<?php 

	$oPreObra = new PreObra();
	$oSubacaoControle = new SubacaoControle();
	$pacFNDE  = $oSubacaoControle->verificaObraFNDE($preid, SIS_OBRAS);
	$arDados  = $oSubacaoControle->recuperarPreObra($preid);
	
	if( $preid ){
	
		$qrpid = pegaQrpidPAC( $preid, 43 );
	
		$pacDados = $oSubacaoControle->verificaTipoObra($preid, SIS_OBRAS);
		$pacFotos = $oSubacaoControle->verificaFotosObra($preid, SIS_OBRAS);
		$pacDocumentos = $oSubacaoControle->verificaDocumentosObra($preid, SIS_OBRAS, $pacDados);
		if($pacFNDE == 'f'){
			$pacDocumentosTipoA = $oSubacaoControle->verificaDocumentosObra($preid, SIS_OBRAS, $pacDados, true);
		}
		$pacQuestionario = $oPreObra->verificaQuestionario($qrpid);
		$boPlanilhaOrcamentaria = $oSubacaoControle->verificaPlanilhaOrcamentaria($preid, SIS_OBRAS, $preid);
		$pacCronograma = $oPreObra->verificaCronograma($preid);
		$pacLatitude   = $oPreObra->verificaLatitudePreObra($preid);
	}

	$docid = prePegarDocid($preid);
	$esdid = prePegarEstadoAtual($docid);
	
	$reformulaMI = verificaMi( $preid );
	
	$boPlanilhaOrcamentaria['faltam'] = $boPlanilhaOrcamentaria['itcid'] - $boPlanilhaOrcamentaria['ppoid'];
	
	$ptoid = $PreObra->ptoid;
	
	$msgPlanilha = 'Falta(m) ' . $boPlanilhaOrcamentaria['faltam'] . ' iten(s) a ser(em) preenchido(s) na planilha orçamentaria.';
	
	if( $ptoid == '' ){
		$ptopreencher = 'f';
	}else{
		$sql = "SELECT ptopreencher FROM obras.pretipoobra WHERE ptoid = $ptoid";
		$ptopreencher = $db->pegaUm( $sql );	
	}

	if( $ptopreencher != 't' ) $msgPlanilha = "Falta salvar a planilha orçamentaria.";

	$arPendencias = Array(	'Dados do terreno' 							=> 'Falta o preenchimento dos dados.',
							'Latitude e Longitude dos Dados do Terreno' => 'Falta o preenchimento da Latitude e Longitude.',
							'Relatório de vistoria' 					=> 'Falta o preenchimento dos dados.',
							'Cadastro de fotos do terreno' 				=> 'Deve conter no mínimo 3 fotos do terreno.',
							'Cronograma físico-financeiro' 				=> 'Falta o preenchimento dos dados.',
							'Documentos anexos' 						=> 'Falta anexar os arquivos.',
							'Projetos - Tipo A' 						=> 'Falta anexar os arquivos.',
							'Itens Planilha orçamentária' 				=> $msgPlanilha,
							'Planilha orçamentária' 					=> $msgPlanilha,
							'Valor da planilha orçamentária' 			=> 'O valor R$ '.formata_valor($boPlanilhaOrcamentaria['valor']).' não confere, deve seve ser maior que R$ '.formata_valor($boPlanilhaOrcamentaria['minimo']).'.');

	$x=0; 
	foreach($arPendencias as $k => $v){
		$cor = ($x % 2) ? 'white' : '#d9d9d9;';  
		if(  ( !$pacDados && $k == 'Dados do terreno' ) || 
			 ( $k == 'Relatório de vistoria' && $pacQuestionario != 22 ) || 
			 ( $k == 'Latitude e Longitude dos Dados do Terreno' && !$pacLatitude ) || 
			 ( $pacFotos < 3 && $k == 'Cadastro de fotos do terreno' ) ||
			 ( $k == 'Itens Planilha orçamentária' && ( $boPlanilhaOrcamentaria['faltam'] > 0 || $boPlanilhaOrcamentaria['ppoid'] < 1 ) ) ||
			 ( $k == 'Planilha orçamentária' && $boPlanilhaOrcamentaria['ppoid'] == 0 && $arDados['ptoprojetofnde'] == 't' && !($reformulaMI)) ||
			 ( $k == 'Valor da planilha orçamentária' && ( str_replace(',','',number_format($boPlanilhaOrcamentaria['valor'],2)) < $boPlanilhaOrcamentaria['minimo'] /*|| str_replace(',','',number_format($boPlanilhaOrcamentaria['valor'],2)) > $boPlanilhaOrcamentaria['maximo']*/) && $pacFNDE == 't' && !($reformulaMI) ) ||
			 ( $k == 'Cronograma físico-financeiro' && !in_array($ptoid, Array(73,74)) && !$pacCronograma && $arDados['ptoprojetofnde'] == 't' && !($reformulaMI) ) ||
			 ( ($pacDocumentosTipoA['arqid'] != $pacDocumentosTipoA['podid'] || !$pacDocumentosTipoA) && $k == 'Projetos - Tipo A' && $arDados['ptoprojetofnde'] == 'f' ) || 
			 ( ($pacDocumentos['arqid'] != $pacDocumentos['podid'] || !$pacDocumentos) && $k == 'Documentos anexos' ) 
			 ){ 
			if(!$boMsg){
			?>
		<tr>
			<td colspan="3" style="text-align:center;font-size:14px;font-weight:bold;color:#900;height:50px;">
				O sistema verificou que alguns dados não foram preenchidos:
			</td>
		</tr>
<?php 		$boMsg = true;
			}
?>
		<tr style="background-color: <?php echo $cor ?>;">
			<td>
<?php 
				switch($k){
					case 'Dados do terreno':
						$aba = 'dados';
						break;
					case 'Relatório de vistoria':
						$aba = 'questionario';
						break;
					case 'Cadastro de fotos do terreno':
						$aba = 'foto';
						break;
					case 'Itens Planilha orçamentária':
						$aba = 'planilhaOrcamentaria';
						break;
					case 'Planilha orçamentária':
						$aba = 'planilhaOrcamentaria';
						break;
					case 'Planilha orçamentária Tipo B 110v':
						$aba = 'planilhaOrcamentaria';
						break;
					case 'Planilha orçamentária Tipo B 220v':
						$aba = 'planilhaOrcamentaria';
						break;
					case 'Planilha orçamentária Tipo C 110v':
						$aba = 'planilhaOrcamentaria';
						break;
					case 'Planilha orçamentária Tipo C 220v':
						$aba = 'planilhaOrcamentaria';
						break;							
					case 'Cronograma físico-financeiro':
						$aba = 'cronograma';
						break;
					case 'Documentos anexos':
						$aba = 'documento';
						break;
					default:
						$aba = "dados";
						break;
				}
?>
				<a href="par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=<?php echo $aba ?>&preid=<?php echo $preid ?>">
				<img border="0" src='/imagens/consultar.gif' onclick='javascript:void(0)'>
				</a>
			</td>
			<td>
				<b><?php echo $k ?></b>
				<br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - 
				<?php echo str_replace("{valor}","R$ ".formata_valor($boPlanilhaOrcamentaria['valor']), $v) ?><br />
			</td>
			<td style="background:white;width:100px;"></td>
		</tr>
<?php 			$x++;
			}
		} 
		if(!$boMsg){ 
?>
		<tr>
			<td colspan="3" style="text-align:center;font-size:14px;font-weight:bold;color:#900;height:50px;">
				O sistema não encontrou pendências de preenchimento. 
			</td>
		</tr>
<?php 	}; ?>
	</table>
</div>

<div id="listaObras" class="aba" style="display:none">
	<?php 
	if( $PreObra->muncodpar ){
		$titulo = 'Municipio';
		$filtro = "AND e.muncod = '{$PreObra->muncodpar}'";
		$esfera = 'M';
	}else{
		$titulo = 'Estado';
		$filtro = "AND e.estuf = '{$PreObra->estufpar}'";
		$esfera = 'E';
	}
	
	montaTituloPreObra("Obras no $titulo",$PreObra->predescricao); 

	$sql = "SELECT
				'<div border=\"0\" onclick=\"redireciona(' || oi.obrid || ')\" 
				onmouseover=\"SuperTitleAjax(window.location.href+\'&req=detalheObras&preid={$preid}&obrid=' || oi.obrid ||
				'&obrdesc=' || coalesce(oi.obrnome,'não informado') ||
				'&plititulo=' || coalesce(plititulo,'não informado') ||
				'&tobadesc=' || coalesce(tobdesc,'não informado') ||
				'&obrvalorprevisto=' || coalesce(obrvalorprevisto,0) ||
				'&endcep=' || coalesce(endcep,'não informado') ||
				'&endlog=' || coalesce(endlog,'não informado') ||
				'&endnum=' || coalesce(endnum,'não informado') ||
				'&endcom=' || coalesce(endcom,'não informado') ||
				'&endbai=' || coalesce(endbai,'não informado') ||
				'&situacao=' || 
				CASE WHEN iexsitdominialimovelregulariza = true
					THEN 'Regularizado'
					ELSE 'Não Regularizado'
				END || 
				'\')\" 
				onmouseout=\"SuperTitleOff( this );\" >' || obrnome || '</div>' as descricao
			FROM
				obras2.obras oi
			INNER JOIN obras2.empreendimento 	emp  ON emp.empid = oi.empid AND emp.empesfera = '$esfera'
			INNER JOIN entidade.endereco 		e    ON e.endid   = oi.endid $filtro
			LEFT  JOIN monitora.pi_obra 		pio  ON pio.obrid = oi.obrid
			LEFT  JOIN monitora.pi_planointerno pipi ON pio.pliid = pipi.pliid 
			LEFT  JOIN obras2.tipoobra 			tob  ON tob.tobid = oi.tobid
			LEFT  JOIN obras2.infraestrutura 	iex  ON iex.iexid = oi.iexid					
			WHERE emp.prfid = 41";
	
	$arrObras = $db->carregar($sql);
?>
	<script type="text/javascript" src="../../includes/remedial.js"></script>
	<script type="text/javascript" src="../../includes/superTitle.js"></script>
	<link rel="stylesheet" type="text/css" href="../../includes/superTitle.css"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">	
		<?php if( !$arrObras ){ ?>
			<tr>
				<td align="center">
				Não existem obras no <?=$titu ?>.
				</td>
			</tr>
		<?php }else{ ?>
			<tr>
				<td align="center">
				<table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem" style="color: rgb(51, 51, 51);">
				<thead>
					<tr>
						<td align="center" valign="top" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" class="title">Nome da Obra
						</td>
					</tr> 
				</thead>
				</table>
				<?php $arrCabecalho = array(""); ?>
				<?php $db->monta_lista_simples($sql,$arrCabecalho,100,20 )?>
				</td>
			</tr>
		<?php } ?>
	</table>
</div>

<div id="documentoFNDE" class="aba" style="display:none">
	<?php montaTituloPreObra("Documentos FNDE",$PreObra->predescricao); ?>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td colspan="2" class="SubTituloCentro">
				<table width="95%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
					<tr bgcolor="">
						<td align="center" width="70%">Descrição</td>
						<td align="center" width="20%">Data de Inclusão</td>
					</tr>
				<?php 
					$x = 0; 
					$sql = "SELECT 
							pdfdescricao as descricao, 
							arqid as codigo,
							to_char(pdfdatainclusao,'DD/MM/YYYY') as data
						FROM 
							obras.preobradocumentosfnde
						WHERE
							pdfstatus = 'A'
							AND preid = ".$preid;
					$arquivos = $db->carregar($sql);
					$arquivos = is_array($arquivos) ? $arquivos : array();
					foreach($arquivos as $arquivo){ 
						$x++;
						$cor = ($x % 2) ? "#F7F7F7" : "white"; 
				?>
					<tr bgcolor="<?php echo $cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $cor ?>';">
						<td align="center"><a class="anexo" id="<?=$arquivo['codigo'] ?>"><?=$arquivo['descricao'] ?></a></td>
						<td align="center"><?=$arquivo['data'] ?></td>
					</tr>
				<?php 
					}
				?>
				</table>
			</td>
		</tr>					
	</table>
</div>

<div id="dadosOrcamentarios" class="aba" style="display:none">
	<?php montaTituloPreObra("Dados Orçamentários",$PreObra->predescricao); ?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="10%" style="text-align: right;" class="SubTituloDireita">Nome da Obra:</td>
			<td width="90%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
				<?=$PreObra->predescricao;?>
			</td>
		</tr>
		<tr>
			<td width="10%" style="text-align: right;" class="SubTituloDireita">Tipo da Obra:</td>
			<td width="90%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
				<?=$db->pegaUm("SELECT ptodescricao FROM obras.pretipoobra WHERE ptoid = {$PreObra->ptoid}");?>
			</td>
		</tr>
	</table>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td style="background-color:#CCCCCC;text-align:center;font-size:12px;" colspan="4"><b>Dados de Empenhos</b></td>
		</tr>
		<tr>
			<td style="background-color:#CCCCCC;text-align:center" width="10%"><b>NE</b></td>
			<td style="background-color:#CCCCCC;text-align:center" width="30%"><b>% Empenho</b></td>
			<td style="background-color:#CCCCCC;text-align:center" width="30%"><b>Valor Empenhado</b></td>
			<td style="background-color:#CCCCCC;text-align:center" width="30%"><b>Situação do Empenho</b></td>
		</tr>
<?php
	$sql = "SELECT ne as empnumero, ((v.saldo * 100) / p.prevalorobra )::NUMERIC(20,2)  as porcentagemempenhado, valorempenho as eobvalorempenho,  empsituacao ,p.prevalorobra 
			FROM par.v_saldo_obra_por_empenho v
			INNER JOIN par.empenho 		e ON e.empid = v.empid
			INNER JOIN obras.preobra 	p ON p.preid = v.preid and p.prevalorobra > 0
			WHERE v.preid = $preid
			ORDER BY
				empnumero";
		
	$dados = $db->carregar($sql);
	if( is_array($dados) ){
		$tot_emp = 0;
		foreach( $dados as $dado ){
			$tot_emp += $dado['eobvalorempenho']; ?>
		<tr>
			<td><b><?=$dado['empnumero']?></b></td>
			<td><?=number_format($dado['porcentagemempenhado'],2,',','.')?> %</td>
			<td>R$ <?=number_format($dado['eobvalorempenho'],2,',','.')?></td>
			<td><?=$dado['empsituacao']?></td>
		</tr>
<? 		} ?>
		<tr>
			<td style="background-color:#CCCCCC;text-align:right"><b>Total:</b></td>
			<td style="background-color:#CCCCCC;"><b><?=number_format((($tot_emp*100) / $dado['prevalorobra'] ),2,',','.')?> %</b></td>
			<td style="background-color:#CCCCCC;"><b>R$ <?=number_format($tot_emp,2,',','.')?></b></td>
			<td style="background-color:#CCCCCC;text-align:center"><b></b></td>
		</tr>
<? 	}else{ ?>
		<tr>
			<td style="color:red;text-align:center" colspan="4">Não possui registros</td>
		</tr>
<?	} ?>
	</table>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td style="background-color:#CCCCCC;text-align:center;font-size:12px;" colspan="4"><b>Dados de Pagamento</b></td>
		</tr>
<?php
	$sql = "SELECT
				pagparcela,
				(( pobvalorpagamento * 100) / pre.prevalorobra  )::NUMERIC(20,2) as pobpercentualpag,
				pobvalorpagamento,
				pagsituacaopagamento,
				emp.empnumero,
				pre.prevalorobra
			FROM
				par.pagamento pag
			INNER JOIN par.empenho 		emp ON emp.empid = pag.empid
			INNER JOIN par.pagamentoobra 	pob ON pob.pagid = pag.pagid
			INNER JOIN obras.preobra 	pre ON pre.preid = pob.preid AND pre.prevalorobra > 0
			WHERE
				pagsituacaopagamento not ilike '%CANCELADO%'
				AND pag.pagstatus = 'A'
				AND pre.preid = $preid
			ORDER BY
				empnumero,
				pagparcela";
		
	$dados = $db->carregar($sql);
	if( is_array($dados) ){
		$numeroempenho = 0;
		$tot_pag = 0;
		foreach( $dados as $dado ){
			if( $numeroempenho != $dado['empnumero'] ){
				$numeroempenho = $dado['empnumero'];
				if( $tot_pag > 0 ){ ?>
		<tr>
			<td style="background-color:#CCCCCC;text-align:right"><b>Total:</b></td>
			<td style="background-color:#CCCCCC;"><b><?=number_format((($tot_pag*100) / $dado['prevalorobra'] ),2,',','.')?> %</b></td>
			<td style="background-color:#CCCCCC;"><b>R$ <?=number_format($tot_pag,2,',','.')?> </b></td>
			<td style="background-color:#CCCCCC;"><b></b></td>
		</tr>
<?					$tot_pag = 0;
				} ?>
		<tr>
			<td colspan="4" ><b>Empenho <?=$dado['empnumero']?></b></td>
		</tr>
		<tr>
			<td style="background-color:#CCCCCC;text-align:center" width="10%"><b>Parcela</b></td>
			<td style="background-color:#CCCCCC;text-align:center" width="30%"><b>% Pago</b></td>
			<td style="background-color:#CCCCCC;text-align:center" width="30%"><b>Valor Pago</b></td>
			<td style="background-color:#CCCCCC;text-align:center" width="30%"><b>Situação do Pagamento</b></td>
		</tr>
<?			} ?>
		<tr>
			<td><?=$dado['pagparcela']?></td>
			<td><?=number_format($dado['pobpercentualpag'],2,',','.')?> %</td>
			<td>R$ <?=number_format($dado['pobvalorpagamento'],2,',','.')?> </td>
			<td><?=$dado['pagsituacaopagamento']?></td>
		</tr>
<? 			$tot_perc_pag 	+= $dado['pobpercentualpag'];
			$tot_pag 		+= $dado['pobvalorpagamento'];
		} ?>
		<tr>
			<td style="background-color:#CCCCCC;text-align:right"><b>Total:</b></td>
			<td style="background-color:#CCCCCC;"><b><?=number_format((($tot_pag*100) / $dado['prevalorobra'] ),2,',','.')?> %</b></td>
			<td style="background-color:#CCCCCC;"><b>R$ <?=number_format($tot_pag,2,',','.')?> </b></td>
			<td style="background-color:#CCCCCC;"><b></b></td>
		</tr>
<?	}else{ ?>
		<tr>
			<td style="color:red;text-align:center" colspan="4">Não possui registros</td>
		</tr>
<?	} ?>
	</table>
</div>

<div id="questionario" class="aba" <?=($_REQUEST['div'] == 'questionario' ? '' : 'style="display:none"' ) ?>>
	<?php if($_REQUEST['div'] == 'questionario'){ form_questionario(); }?>
</div>

<div id="analise_engenharia" class="aba" <?=($_REQUEST['div'] == 'analise_engenharia' ? '' : 'style="display:none"' ) ?>>
	<?php if( $_REQUEST['div'] == 'analise_engenharia' ){ form_analise_engenharia(); }?>
</div>