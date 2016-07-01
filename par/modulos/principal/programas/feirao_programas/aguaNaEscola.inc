<?php

if(!$_SESSION['par']['adpid'] || !$_SESSION['par']['prgid']){
	echo '<script type="text/javascript"> 
    		alert("Sessão Expirou.\nFavor selecione o programa novamente!");
    		window.location.href="par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa";
    	  </script>';
    die;
}


//verifica se atingiu a meta de 1000 escolas aderidas.
$sql = "SELECT count(pfa.paeid) as total
		FROM par.pfaguaescola pfa
		WHERE pfa.paeparticipa = 'S'";
$totalEscolasMil = $db->pegaUm($sql);

if($_REQUEST['paeid']){

	ini_set("memory_limit","1024M");
	
	if($_REQUEST['requisicao'] == 'salvarJustificativa'){
		
		//deleta todas as fotos
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		
		$sql = "select arqid from par.pffotosaguaescola
				where adpid = ".$_SESSION['par']['adpid']."
				and paeid = ".$_REQUEST['paeid'];
		$dadosFotos = $db->carregar($sql);
		
		if($dadosFotos){
			foreach($dadosFotos as $f){
				$arqid = $f['arqid'];
				
				$file = new FilesSimec("pffotosaguaescola",array(),"par");
				$file->excluiArquivoFisico($arqid);
				$sql = "delete from par.pffotosaguaescola where arqid = $arqid;";
				$sql.= "delete from public.arquivo where arqid = $arqid;";
				$db->executar($sql);
			}	
			unset($arqid);		
		}
		
		
		//atualiza justificativa
		$sql = "UPDATE par.pfaguaescola
   				SET paeparticipa='N', paejustificativa='".$_REQUEST['paejustificativa']."'
 				WHERE  paeid = ".$_REQUEST['paeid'];
		$db->executar($sql);
		$db->commit();
		
		echo '<script>alert("Operação Efetuada com Sucesso.");</script>';
			
	}
	
	if($_REQUEST['requisicao'] == 'salvarFotos'){
		
		//insere sim para paeparticipa e deleta a justificativa
		$sql = "UPDATE par.pfaguaescola
   				SET paejustificativa=null, paeparticipa='S'
 				WHERE  paeid = ".$_REQUEST['paeid'];
		$db->executar($sql);
			
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		
		extract($_POST);
		
		if( $_FILES['arquivo']['tmp_name'] ){
			$arrCampos = array(
							"adpid" => $_SESSION['par']['adpid'],
							"paeid" => $_REQUEST['paeid'],
							"usucpf" => "'{$_SESSION['usucpf']}'",
							"pfostatus" => "'A'",
							"pfodatainclusao" => "now()"
						      );
			$file = new FilesSimec("pffotosaguaescola", $arrCampos, "par");
			$file->setUpload($arqdescricao, "arquivo");
			
			//header("Location: par.php?modulo=principal/programas/feirao_programas/aguaNaEscola&acao=A");
			//die;
		}else{
			$_SESSION['par']['mgs'] = "Não foi possível realizar a operação!";
			//header("Location: par.php?modulo=principal/programas/feirao_programas/aguaNaEscola&acao=A");
			//die;
		}
		
	}
	
	if($_REQUEST['requisicaoAjax'] == 'removerFoto'){
		
		$arqid = $_GET['arqid'];
		
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		
		$file = new FilesSimec("pffotosaguaescola",array(),"par");
		$file->excluiArquivoFisico($arqid);
		$sql = "delete from par.pffotosaguaescola where arqid = $arqid;";
		$sql.= "delete from public.arquivo where arqid = $arqid;";
		$db->executar($sql);
		$db->commit();
		
		die;
	}

	$_SESSION['downloadfiles']['pasta'] = array("origem" => "par","destino" => "par");
	$_SESSION['imgparams']  			= array("filtro" => "pfostatus = 'A'", "tabela" => "par.pffotosaguaescola");
	$_SESSION['sisarquivo'] 			= 'par';
	
	
	//verifica permissao de inserção/exclusão de fotos
	$docid = pgPegarDocid( $_SESSION['par']['adpid'] );
	$esdid = prePegarEstadoAtual( $docid );
	if( $esdid == WF_TIPO_AGUANAESCOLA_EM_CADASTRAMENTO ){
		$habilitado = 'S';
	}
	else{
		$habilitado = 'N';
	}
		
	
	$sql = "select prgdsc from par.programa where prgid = ".$_SESSION['par']['prgid'];
	$nomePrograma = $db->pegaUm($sql);
	
	if($_SESSION['par']['muncod']){
		
		$sql = "select mundescricao, estuf from territorios.municipio where muncod = '{$_SESSION['par']['muncod']}'";
		$rsMunicipio = $db->pegaLinha($sql);
		$descricao = "MUNICÍPIO: ".$rsMunicipio['mundescricao'].' - '.$rsMunicipio['estuf'];
		
	}else{
		
		$descricao = "ESTADO: ".$_SESSION['par']['estuf'];
	}
		
	monta_titulo($nomePrograma,$descricao);
	
	//recupera flag sim/nao (Deseja que esta Escola participe do Programa Água na Escola)
	$sql = "SELECT paeparticipa, paejustificativa FROM par.pfaguaescola where paeid = ".$_REQUEST['paeid'];
	$dadosParticipa = $db->pegaLinha($sql);
	if($dadosParticipa) {
		extract($dadosParticipa);
		$paeparticipa2 = $paeparticipa;
	}
	
	$paeparticipa = $_REQUEST['paeparticipa'] ? $_REQUEST['paeparticipa'] : $paeparticipa;
	
	
	//permissao edição
	$programa = pegarProgramaDisponivel();
	$programa = $programa ? $programa : array();
	if( !in_array(PROGRAMA_AGUANAESCOLA, $programa) ){
		$habilitado = 'N';
	}
	
	?>
	<script language="JavaScript" src="/includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
	
	<body onLoad="opener.location.reload();">
	<form name="formulario" method="post">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	    <tr>
	    	<td valign="top" class="SubTituloDireita" width="40%">
	    		Escola:  
			</td>
			<td>
				<? if($_REQUEST['paeid']){
	    		   		echo $db->pegaUm("select entnome from par.pfaguaescola pfa
	    		   						  inner join entidade.entidade ent on ent.entcodent = pfa.entcodent
	    		   						  where paeid = '".$_REQUEST['paeid']."'"); 
				   }
				?>
			</td>
		</tr>
		<tr>
	    	<td valign="top" class="SubTituloDireita" width="40%">
	    		Deseja que esta Escola participe <br>do Programa Água na Escola:  
			</td>
			<td>
				<input type="radio" name="paeparticipa" value='S' <?if($paeparticipa == 'S'){echo 'checked';}?> <?if($habilitado == 'N'){echo 'disabled';}?> onclick="verificaSim();"> SIM
				&nbsp;&nbsp;&nbsp;
				<input type="radio" name="paeparticipa" value='N' <?if($paeparticipa == 'N'){echo 'checked';}?> <?if($habilitado == 'N'){echo 'disabled';}?> onclick="location.href='par.php?modulo=principal/programas/feirao_programas/aguaNaEscola&acao=A&paeid=<?=$_REQUEST['paeid']?>&paeparticipa=N'"> NÃO
			</td>
		</tr>
	</table>
	<?if($paeparticipa == 'N'){?>
		<input type="hidden" name="requisicao" value="salvarJustificativa" />
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		    <tr>
		    	<td valign="top" class="SubTituloDireita" width="40%">
		    		Justificativa:  
				</td>
				<td>
					<?=campo_textarea("paejustificativa", "S", $habilitado, "", 60, 8, 4000)?>
				</td>
			</tr>
			<tr>
				<td class="SubtituloDireita" ></td>
				<td class="SubtituloEsquerda" >
					<?if($habilitado == 'S'){?>
					<input type="button" name="btn_salvar_just" value="Salvar" onclick="salvarJust();">
					<?}?>
					<input type="button" name="btn_cancelar_just" value="Cancelar" onclick="window.close();">
				</td>
			</tr>
		</table>
		
	<?}elseif($paeparticipa == 'S'){?>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		    <tr>
		    	<td valign="top">
					<!-- 
					<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					-->
					
						<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
						<?php $arrSalas = array( array('salid' => '1', 'tisdescricao' => 'Adicionar Fotos' ) ); ?>
						<?php if($arrSalas): ?>
							<?php $i=0 ?>
							<?php foreach($arrSalas as $sala): ?>
								<?php echo $i%2 == 0 ? "<tr>" : "" ?>
								<td width="50%" valign="top" id="td_sala_<?php echo $sala['salid'] ?>">
									<fieldset id="field_sala_<?php echo $sala['salid'] ?>" class="field_foto" >
										<legend><?php echo $sala['tisdescricao'] ?></legend>
										<table width="100%" >
											<tr>
												<td valign="top">
													<?php 
		
														if($_REQUEST['paeid']){
															$sql = "select
																			arq.arqid,
																			arq.arqnome||'.'||arq.arqextensao,
																			arq.arqdescricao,
																			arq.arqtamanho
																		from 
																			par.pffotosaguaescola fot
																		inner join
																			public.arquivo arq ON arq.arqid = fot.arqid
																		where
																			fot.adpid = ".$_SESSION['par']['adpid']."
																		and
																			fot.paeid = ".$_REQUEST['paeid']."
																		and
																			fot.pfostatus = 'A'";
			
															$arrFotos = $db->carregar($sql);
														}
			
													?>
													<?php if($arrFotos): ?>
														<?php foreach($arrFotos as $foto): ?>
															<?php $pagina = floor($n/16); ?>
															<?php
																	$imgend = APPRAIZ.'arquivos/'.(($_REQUEST["_sisarquivo"])?$_REQUEST["_sisarquivo"]:$_SESSION["sisarquivo"]).'/'. floor($foto['arqid']/1000) .'/'.$foto['arqid'];
															
																	if(is_file($imgend)){
																		$img_max_dimX = 100;
																		$img_max_dimY = 85;
																		
																		$imginfo = getimagesize($imgend);
																		
																		$width = $imginfo[0];
																		$height = $imginfo[1];
																	
																		if (($width >$img_max_dimX) or ($height>$img_max_dimY)){
																			if ($width > $height){
																			  	$w = $width * 0.9;
																				  while ($w > $img_max_dimX){
																					  $w = $w * 0.9;
																				  }
																				  $w = round($w);
																				  $h = ($w * $height)/$width;
																			  }else{
																				  $h = $height * 0.9;
																				  while ($h > $img_max_dimY){
																					  $h = $h * 0.9;
																				  }
																				  $h = round($h);
																				  $w = ($h * $width)/$height;
																			  }
																		}else{
																			  $w = $width;
																			  $h = $height;
																		}
																		
																		$tamanho = " width=\"$w\" height=\"$h\" ";
																	}else{
																		$tamanho = "";
																	}
															?>
															<div id="div_foto_<?php echo $foto['arqid'] ?>" class="div_foto">
																<?php if($habilitado == 'S'){ ?>
																	<img src="../imagens/fechar.jpeg" title="Remover Foto" class="fechar" onclick="removerFoto('<?php echo $foto['arqid'] ?>')" />
																<?php }else{ ?>
																	<img src="../imagens/fechar.jpeg" title="Remover Foto" class="fechar" onclick="alert('Acesso Negado! \nSó é possivel excluir fotos no estado: Em Cadastramento.');" />
																<?php } ?>
																<img onClick="abrirGaleria('<?php echo $foto['arqid'] ?>','<?php echo $pagina ?>','<?php echo $sala['salid'] ?>','<?php echo $_REQUEST['paeid'] ?>')" <?php echo $tamanho ?> onmouseover="return escape('<b>Descrição:</b> <?php echo $foto['arqdescricao'] ?><br /><b>Tamanho(Kb):</b> <?php echo $foto['arqtamanho'] ? round($foto['arqtamanho']/1024,2) : "N/A" ?>');" class="img_foto" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $foto['arqid'] ?>&newwidth=100&newheight=85&_sisarquivo=<?=(($_REQUEST["_sisarquivo"])?$_REQUEST["_sisarquivo"]:$_SESSION["sisarquivo"]) ?>" />
															</div>
														<?php endforeach; ?>
													<?php endif; ?>
												</td>
												<?php if($habilitado == 'S' && count($arrFotos) < 20){ ?>
													<td valign="top" width="10" align="right"><img class="img_add_foto" src="../imagens/gif_inclui.gif" title="Adicionar Foto" onclick="addFoto('<?php echo $sala['salid'] ?>')"></td>
												<?php } ?>
											</tr>
										</table>
									</fieldset>
								</td>
								<?php echo $i%2 == 1 ? "</tr>" : "" ?>
								<?php $i++ ?>
							<?php endforeach; ?>
						<?php endif; ?>
						</table>
						
					
					
				</td>
				
			</tr>
		</table>
	<?}?>
	</form>
	</body>
<?php $html = '<form id="form_upload" name="form_upload" method=post enctype="multipart/form-data" >
					<input type="hidden" name="salid" id="salid" value="" />
					<input type="hidden" name="requisicao" value="salvarFotos" />
					<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
						<tr>
							<td width="30%" class="SubtituloDireita" >Foto:</td>
							<td><input type="file" name="arquivo" /></td>
						</tr>
						<tr>
							<td class="SubtituloDireita" >Descrição:</td>
							<td>'.campo_texto("arqdescricao","S","S","",40,60,"","","","","","","").'</td>
						</tr>
						<tr>
							<td class="SubtituloDireita" ></td>
							<td class="SubtituloEsquerda" >
								<input type="button" name="btn_upload" value="Salvar" />
								<input type="button" name="btn_cancelar" value="Cancelar" />
							</td>
						</tr>
					</table>
			   </form>' ?>
<?php popupAlertaGeral($html,"400px","120px","div_sub_foto","hidden"); ?>
	
	
	<?

}else{
	
	include APPRAIZ . "includes/cabecalho.inc";
	echo '<br/>';
	
	$sql = "select prgdsc from par.programa where prgid = ".$_SESSION['par']['prgid'];
	$nomePrograma = $db->pegaUm($sql);
	
	if($_SESSION['par']['muncod']){
		
		$sql = "select mundescricao, estuf from territorios.municipio where muncod = '{$_SESSION['par']['muncod']}'";
		$rsMunicipio = $db->pegaLinha($sql);
		$descricao = $rsMunicipio['mundescricao'].' - '.$rsMunicipio['estuf'];
		
	}else{
		
		$descricao = $_SESSION['par']['estuf'];
	}
		
	monta_titulo($nomePrograma,$descricao);
	echo '<br/>';
		
	echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/aguaNaEscola&acao=A');
	monta_titulo( 'Fotos', 'Observação: Anexar no mínimo 3 fotos para cada escola.' );
	
	?>
	<body>
	<form name="formulario" method="post">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	    <tr>
	    	<td valign="top">
	    		<br>
				<!-- 
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				-->
				
				<?
				
					
					if($_SESSION['par']['itrid'] == 2){
						$stWhere = "AND eed.muncod = '{$_SESSION['par']['muncod']}'";
					}else{
						$stWhere = "AND eed.estuf = '{$_SESSION['par']['estuf']}'";
					}
					
					$sql = "SELECT
									'<center><img src=\"../imagens/gif_inclui.gif\" style=\"cursor:pointer;\" border=\"0\" onclick=\"addFotoEnt(\'' || pfa.paeid || '\');\" title=\"Adicionar Foto\"></center>' as acao,
									CASE WHEN pfa.paeparticipa = 'N' THEN
											'Não Participa'
										 ELSE
											CASE WHEN totalfotos > 0 THEN
													CASE WHEN totalfotos < 3 THEN
												 		'<div id=\"qtd_' || pfa.paeid || '\"><FONT COLOR=RED>'||totalfotos||'</FONT></div>'
													 ELSE
														'<div id=\"qtd_' || pfa.paeid || '\"><FONT COLOR=BLUE>'||totalfotos||'</FONT></div>'
													END
												ELSE
													'<div id=\"qtd_' || pfa.paeid || '\"><FONT COLOR=RED>0</FONT></div>'
											END	
									END as qtdfotos,
									ent.entnome as descricao
								  FROM par.pfaguaescola pfa
								  INNER JOIN entidade.entidade ent ON ent.entcodent = pfa.entcodent
								  INNER JOIN entidade.endereco eed ON ent.entid = eed.entid {$stWhere}
								  INNER JOIN entidade.funcaoentidade fue ON ent.entid = fue.entid AND fue.funid = ".FUNID_ESCOLA."
								  LEFT JOIN (select paeid, count(pfoid) as totalfotos from par.pffotosaguaescola group by paeid) fot on fot.paeid = pfa.paeid
								  WHERE	ent.entstatus = 'A'
								  AND ent.entcodent IS NOT NULL
								  AND ent.entnome IS NOT NULL
								  AND ent.tpcid = ".($_SESSION['par']['itrid'] == 2 ? 3 : 1)."
								  
								  ORDER BY 2 ASC";
						
					//}
					//dbg($sql,1);
					$cabecalho = array("Ação","Qtd. Fotos","Escolas");
					$tamanho		= array( '10%', '10%', '80%' );															
					$alinhamento	= array( 'center', 'center', 'left' );													
					$db->monta_lista( $sql, $cabecalho, 500, 10, 'N', 'center', '', '', $tamanho, $alinhamento);
				
				?>
			</td>
			<td width="80" valign="top" align="right">
				<?
				include APPRAIZ ."includes/workflow.php";
				$dados_wf = array('adpid' => $_SESSION['par']['adpid']);
				wf_desenhaBarraNavegacao(pgAguaEscolaCriarDocumento( $_SESSION['par']['adpid'] ), $dados_wf);
				?>
			</td>
		</tr>
	</table>	
	</form>
	</body>
	<?
	
}
?>

<style>
	.field_foto {text-align:right}
	.field_foto legend{font-weight:bold;}
	.img_add_foto{cursor:pointer}
	.hidden{display:none}
	.img_foto{border:0;margin:5px;cursor:pointer;margin-top:-5px}
	.div_foto{width:110px;height:90px;margin:3px;border:solid 1px black;float:left;text-align:center;background-color:#FFFFFF}
	.fechar{position:relative;margin-left:104px;top:-6px;cursor:pointer}
</style>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
$(function() {
	 $('[name=btn_upload]').click(function() {
	 	var erro = 0;
	 	if(!$('[name=arquivo]').val()){
	 		alert('Favor selecionar a foto!');
	 		erro = 1;
	 		return false;
	 	}
	 	if(!$('[name=arqdescricao]').val()){
	 		alert('Favor informar uma descrição!');
	 		erro = 1;
	 		return false;
	 	}
	 	if(erro == 0){
	 		$('[name=btn_upload]').val("Carregando...");
	 		$('[name=btn_cancelar]').val("Carregando...");
	 		$('[name=btn_upload]').attr("disabled","disabled");
	 		$('[name=btn_cancelar]').attr("disabled","disabled");
	 		$('[name=form_upload]').submit();
	 	}
	 });
	 
	 $('[name=btn_cancelar]').click(function() {
	 	$('[id=div_sub_foto]').hide();
	 });
	 
	  $('[name=btn_salvar_just]').click(function() {
	 	var erro = 0;
	 	if(!$('[name=paejustificativa]').val()){
	 		alert('Favor informar a justificativa!');
	 		erro = 1;
	 		return false;
	 	}
	 	if(erro == 0){
	 		$('[name=btn_salvar_just]').val("Carregando...");
	 		$('[name=btn_cancelar_just]').val("Carregando...");
	 		$('[name=btn_salvar_just]').attr("disabled","disabled");
	 		$('[name=btn_cancelar_just]').attr("disabled","disabled");
	 		$('[name=formulario]').submit();
	 	}
	 });
	 
	 <?php if($_SESSION['par']['mgs']): ?>
	 	alert('<?php echo $_SESSION['par']['mgs'] ?>');
	 	<?php unset($_SESSION['par']['mgs']) ?>
	 <?php endif; ?>
});

function addFoto(salid)
{
	$('[name=salid]').val(salid);
	$('[id=div_sub_foto]').show();
	$( 'html, body' ).animate( { scrollTop: 0 }, 'slow' ); 
}

function addFotoEnt(paeid)
{
	window.open("par.php?modulo=principal/programas/feirao_programas/aguaNaEscola&acao=A&paeid=" + paeid,"imagem","width=650,height=280,resizable=yes"); 
}

function removerFoto(arqid)
{
	if(confirm("Deseja realmente excluir esta foto?")){
		$.ajax({
		url: window.location + "&requisicaoAjax=removerFoto&arqid=" + arqid,
		success: function(data) {
			$('[id=div_foto_' + arqid + ']').remove();
			location.href="par.php?modulo=principal/programas/feirao_programas/aguaNaEscola&acao=A&paeid=<?=$_REQUEST['paeid']?>";
	    	}
		});
	}
}

function abrirGaleria(arqid,pagina,salid,paeid)
{
	//window.open("../slideshow/slideshow/index.php?_sisarquivo=par&getFiltro=1&pagina=" + pagina + "&arqid=" + arqid + "&salid=" + salid + "&adpid=" + adpid,"imagem","width=850,height=600,resizable=yes");
	window.open("../slideshow/slideshow/index.php?_sisarquivo=par&getFiltro=1&pagina=" + pagina + "&arqid=" + arqid + "&paeid=" + paeid,"imagem","width=850,height=600,resizable=yes");
}
	
function verificaSim()
{
	alert("É necessário incluir no mínimo 3 fotos para esta escola.");
	location.href='par.php?modulo=principal/programas/feirao_programas/aguaNaEscola&acao=A&paeid=<?=$_REQUEST['paeid']?>&paeparticipa=S';
	/*
	var totalEscolas = "<?=$totalEscolasMil?>";
	var limiteEscolas = 1000;
	
	if("<?=$paeparticipa2?>" == "S") limiteEscolas = limiteEscolas +1;
 
	if(parseFloat(totalEscolas) <= limiteEscolas){
		location.href='par.php?modulo=principal/programas/feirao_programas/aguaNaEscola&acao=A&paeid=<?=$_REQUEST['paeid']?>&paeparticipa=S';
	}
	else{
		alert("Esta Escola não pode participar do programa Água na Escola, pois sua meta já atingiu 1000 Escolas aderidas. \n\nÉ necessário que você opte por não participar para esta e as demais Escolas que não possua fotos para Enviar para análise!");
		location.href='par.php?modulo=principal/programas/feirao_programas/aguaNaEscola&acao=A&paeid=<?=$_REQUEST['paeid']?>&paeparticipa=N';		
	}
	*/
}
	

</script>

	