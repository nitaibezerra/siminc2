<?php
$_SESSION['rehuf_var']['entid'] = $_GET['entid'] ? $_GET['entid'] : $_SESSION['rehuf_var']['entid'];
$entid = $_SESSION['rehuf_var']['entid'] ? $_SESSION['rehuf_var']['entid'] : $_GET['entid'];

if($_GET['arquivo'])
{
	include_once APPRAIZ . "includes/classes/file.class.inc";
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec();
	$file-> getDownloadArquivo($_GET['arquivo']);
	exit;
}

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

/* montando o menu */
echo montarAbasArray(carregardadosmenurehuf(), "/rehuf/rehuf.php?modulo=principal/prestacaocontas&acao=A");

monta_titulo( "HU", "Prestação de Contas");
monta_cabecalho_rehuf($entid);
if($entid){
	include_once APPRAIZ . "includes/classes/modelo/entidade/Entidade.class.inc";
	include_once APPRAIZ . "includes/classes/Modelo.class.inc";
	$e = new Entidade();
		$e->carregarPorId( $entid );
		extract($e->getDados());
		$dadosentidade = $e->buscarUniversidadeByHospital($entid);
}
$cabecalho = $entungcod. "-" .$dadosentidade['ifes']. "-" .$dadosentidade['entnome'];
?>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
	jQuery(function() {
			<?php if($_SESSION['rehuf']['altert']): ?>
				alert('<?php echo $_SESSION['rehuf']['altert'] ?>');
				<?php unset($_SESSION['rehuf']['altert']) ?>
			<?php endif; ?>
				escondetr(<?php echo $plitid ?>);
		});
		
	function escondetr(plitid){
		if(plitid == '1'){
			jQuery('.planotrabalho').hide();
			jQuery('.portaria').show();
		}else if(plitid == '2'){
			jQuery('.portaria').hide();
			jQuery('.planotrabalho').show();
		}else{
			jQuery('.planotrabalho').hide();
			jQuery('.portaria').hide();
		}
	}
	
	function responderformulario(){
		var erro = 0;
		if(!jQuery("[name='plitid']").val()){
				alert('Favor informar um Tipo');
				erro = 1;
				return false;
		}
		if(!jQuery("[name='pi']").is(":checked")){
				alert('Favor informar um Plano Interno');
				erro = 1;
				return false;
		}
		if(erro == 0){
			//jQuery("#formulario").submit();
			jQuery('loader-container').show();
			irformulario();
		}	
	}
	
	function irformulario(){
		//Portaria
		if(jQuery("[name='plitid']").val() == '1'){
			var prtid = "";
			jQuery('input:radio[name=pi]').each(function() {
		                        //Verifica qual está selecionado
		                        if (jQuery(this).is(':checked'))
		                            prtid = parseInt(jQuery(this).val());
		                    })
			var url = "rehuf.php?modulo=principal/formularioPrestacaoContas&acao=A&prtid="+prtid;
				jQuery(location).attr('href',url);
		//Plano Trabalho		
		}else if(jQuery("[name='plitid']").val() == '2'){
				var pltid = "";
				jQuery('input:radio[name=pi]').each(function() {
		                        //Verifica qual está selecionado
		                        if (jQuery(this).is(':checked'))
		                            pltid = parseInt(jQuery(this).val());
		                    })
			var url = "rehuf.php?modulo=principal/formularioPrestacaoContas&acao=A&pltid="+pltid;
				jQuery(location).attr('href',url);
			}
		
	}
</script>
<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>
<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>
	<tr id="tr_nomehospital">
		<td class="SubTituloCentro"><?=$cabecalho  ?></td>
	</tr>
</table>
<form method="POST" id="formulario" name="formulario">
	<input type="hidden" name="requisicao" value="salvarPrestacaoContas">
	<input type="hidden" name="pliid" id="pliid" value="<?=$pliid?>">
	<input type="hidden" name="prtid" id="prtid" value="<?=$prtid?>">
	<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>
		<tr>
			<td width="30%" class="SubTituloDireita">Tipo:</td>
			<td>
				<?php
					$sql = "SELECT
							 p.plitid AS codigo,
							 p.plitdsc AS descricao
							FROM
							 rehuf.planointernotipo p
							WHERE
							 p.plitstatus = 'A'
							ORDER BY
							 p.plitdsc desc";
					$db->monta_combo("plitid",$sql, 'S' ,'Selecione...','escondetr','');
				?>
				<?= obrigatorio(); ?>
			</td>
		</tr>
		<tr class="portaria">
			<td width="25%" class="SubTituloDireita"></td>
			<td>
			<?php
				$sql = "SELECT DISTINCT
							pi.plinumero,
							prt.*,
							vp.vpiid,
							arq.arqid,
							arq.arqnome
						FROM
								rehuf.vinculoplanointerno vp
						LEFT JOIN 
								rehuf.planointerno pi ON vp.pliid=pi.pliid
						LEFT JOIN 
								rehuf.portaria prt ON vp.prtid=prt.prtid
						LEFT JOIN
								public.arquivo arq ON arq.arqid = vp.arqid AND arq.arqstatus = 'A'
						WHERE 
							1 = 1
						AND 
							prt.prtid is not null
						AND
							vp.pltid is null
						
						GROUP BY
								prt.prtid,prt.prtnumero,prt.prtdata,prt.prtdtpublicacao,prt.prtnumerodou,pi.plinumero,vp.vpiid,arq.arqnome,arq.arqid";
				$arrPortaria = $db->carregar($sql);
			 ?>
			 </td>
		</tr>
		<?php if($arrPortaria){	?>
			<tr class="portaria">
				<td width="25%" class="SubTituloDireita"></td>
				<td align="center" colspan="2">
				
					<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="left" style="width: 100%">
						<?php
							//	Início foreach portaria
							$n = 0;
							foreach($arrPortaria as $dado){
						 ?>
						<tr>
							<td width="3%">
								<input type="radio" id="<?=$dado['prtid'] ?>" name="pi" value="<?=$dado['prtid'] ?>"/>
							</td>
							<td width="10%">
								<?php echo $arrLista[$n]['plinumero'] = mb_strtoupper($dado['plinumero']); ?>
							</td>
							<td width="55%">
								<?php echo $arrLista[$n]['descricao'] = "Portaria Nº {$dado['prtnumero']}, de ".exibeDataPorExtenso($dado['prtdata'])." - DOU {$dado['prtnumerodou']}, de ".formata_data($dado['prtdtpublicacao'])."." ?>
							</td>
							<td>
								<?php
									$arrLista[$n]['pi'] = $dado['pi'];
									if($dado['arqid']){
										echo $arrLista[$n]['arquivo'] = "<a href='rehuf.php?modulo=principal/prestacaocontas&acao=A&arquivo=".$dado['arqid']."' ><img border=0 src=\"../imagens/anexo.gif\" title=\"{$dado['arqnome']}\" /> {$dado['arqnome']}</a> ";
									}else{
										echo $arrLista[$n]['arquivo'] = "N/A";
									} 
								?>
							</td>
						</tr>
						 <?php $n++;
							}// Fim foreach portaria?>
					</table>
				</td>
			</tr>
		<?php }//fim if ?>
		<tr class="planotrabalho">
			<td width="25%" class="SubTituloDireita"></td>
			<td>
				<?php
					$sql = "SELECT DISTINCT
										pi.plinumero,
										plt.*,
										vp.vpiid,
										arq.arqid,
										arq.arqnome
							
							FROM
									rehuf.vinculoplanointerno vp
							LEFT JOIN 
									rehuf.planointerno pi ON vp.pliid=pi.pliid
							INNER JOIN 
									rehuf.planotrabalho plt ON vp.pltid=plt.pltid
							LEFT JOIN
									public.arquivo arq ON arq.arqid = vp.arqid AND arq.arqstatus = 'A'
							WHERE 
									1=1
							AND
								plt.pltid is not null
							AND
								vp.prtid is null
							GROUP BY
									plt.pltid,plt.plttitulo,pi.plinumero,vp.vpiid,arq.arqnome,arq.arqid";
					$arrPlanotrabalho = $db->carregar($sql);
				 ?>
			</td>
		</tr>
		<?php if($arrPlanotrabalho){ ?>
			<tr class="planotrabalho">
				<td width="25%" class="SubTituloDireita"></td>
				<td align="left" colspan="2">
					<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="left" style="width: 100%">
						<?php 
							//Início foreach portaria
							$n = 0; 
						  foreach($arrPlanotrabalho as $dado){ ?>
						<tr>
							<td width="3%">
								<input type="radio" id="<?=$dado['pltid'] ?>" name="pi" value="<?=$dado['pltid'] ?>"/>
							</td>
							<td width="10%">
								<?php echo $arrLista[$n]['plinumero'] = $dado['plinumero']; ?>
							</td>
							<td width="55%">
								<? echo $arrLista[$n]['descricao'] = $dado['plttitulo']; ?>
							</td>
							<td>
								<?php
									$arrLista[$n]['pi'] = $dado['pi'];
									if($dado['arqid']){
										echo $arrLista[$n]['arquivo'] = "<a href='rehuf.php?modulo=principal/prestacaocontas&acao=A&arquivo=".$dado['arqid']."' ><img border=0 src=\"../imagens/anexo.gif\" title=\"{$dado['arqnome']}\" /> {$dado['arqnome']}</a> ";
									}else{
										echo $arrLista[$n]['arquivo'] = "N/A";
									} 
								?>
							</td>
						</tr>
						<?php $n++;
						}// Fim foreach portaria  ?>
					</table>
				</td>
			</tr>
		<?php }//fim if ?>
		<tr>
			<td class="SubTituloDireita"></td>
			<td>
				<input type="button" name="btn_salvar" value="Responder Formulário" id="irformulario" onclick="responderformulario()" />
			</td>
	  </tr>
	</table>
</form>
