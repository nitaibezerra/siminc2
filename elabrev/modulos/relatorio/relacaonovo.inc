<?php 



$modulo=$_REQUEST['modulo'] ;//


if ($_REQUEST['prgid']) $_SESSION['prgid'] = $_REQUEST['prgid'];
else $_REQUEST['prgid'] =$_SESSION['prgid'];

if ($_REQUEST['acaid']) $_SESSION['acaid']= $_REQUEST['acaid']; 
else $_REQUEST['acaid'] = $_SESSION['acaid'];


	unset($_SESSION['rsdados']);
  
	$acacodid=$_SESSION['acaid'];
	$exclusao=1;
	$migracao=1;
	$fusao=1;
	$alteracao=1;
		//include "geraproposta.inc";
	?>
			<script language="JavaScript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<?

include  APPRAIZ."elabrev/modulos/relatorio/acao/geraproposta.inc.bck.inc";	


    // /home/simec/sistemas/modulos/relatorio/acao/geraproposta.inc)

///var /home/simec/sistemas/elabrev/modulos/relatorio/acao/geraproposta.inc
/*
	

$sql = "select a.*, b.prgdsc from ".$_SESSION['sisdiretorio'].".ppaacao_proposta a, ".$_SESSION['sisdiretorio'].".ppaprograma_proposta b where a.acastatus in ('A','N') and b.prgstatus='A' and a.prgid = b.prgid and a.prsano ='" .$_SESSION['exercicio']."' and a.prgid=".$_SESSION['prgid'];

if ($_REQUEST['acacod'] )
{
    $sql = $sql. "  and a.acacod='".$_REQUEST['acacod']."' limit 1";
}
elseif ($_SESSION['acaid'])
{
	$sql = $sql. "  and a.acaid=".$_SESSION['acaid'];
}

$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);

$nl=$nlinhas;

if ($nlinhas >= 0) {
    $res =  $db->carrega_registro($RS,$_POST['registro']);
    if(is_array($res)){
       foreach($res as $k=>$v) {${$k}=$v;};
            $_SESSION['prgid'] = $prgid;
            $_SESSION['acaid']= $acaid;

            if ($acasnmetanaocumulativa=='t') $tipometa ='Não cumulativa';
            if ($acasnmetanaocumulativa=='f') $tipometa ='Cumulativa';
            if ($acasnmetanaocumulativa=='') $tipometa ='Não informado';
       }
	// encontrar o desafio (macro-objetivo)
        $sql= "select prodsc from produto where procod='".$procod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar descriÃ§Ã£o da unidade de medida
        $sql= "select unmdsc from unidademedida where unmcod='".$unmcod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar descriÃ§Ã£o do tipo de aÃ§Ã£o
        $sql= "select tacdsc from tipoacao where taccod='".$taccod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar a funÃ§Ã£o
       if ($funcod) {
        $sql= "select fundsc from elabrev.ppafuncao where funcod='".$funcod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        }
        // encontrar a subfunÃ§Ã£o
       if ($sfucod) {
        $sql= "select sfudsc from elabrev.ppasubfuncao where sfucod='".$sfucod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
       }
    // encontrar o Ã³rgÃ£o responsÃ¡vel
        $sql= "select orgdsc from elabrev.ppaorgao where orgcod='".$orgcod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

    // encontrar o orgao responsÃ¡vel
        $sql= "select orgdsc from elabrev.ppaorgao where substr(orgcod,0,3)='".substr($unicod,0,2)."'";

        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        
    // encontrar a unidade responsÃ¡vel
        $sql= "select unicod||'-'||unidsc as unidsc from unidade where unicod='".$unicod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    // encontrar o produto
        $sql= "select prodsc from produto where procod='".$procod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    // encontrar a meta
        //$sql= "select fisqtdeprevistoano as meta from elabrev.dadofisico where acaid='$acaid' and prsano='".$_SESSION['exercicio']."'";
       //$res = $db->recuperar($sql);
       // if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    // encontrar a esfera
       if ($esfcod) {
        $sql= "select esfdsc from esfera where esfcod='".$esfcod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        }
 } else
 {
   ?>
<script>
    alert('Código da Ação inexistente.! \nVocê pode estar tentando abrir ação de outro exercício.');
    history.back();
</script>
   <?
   exit();
 }


function cabecalhoBrasao()
{
	global $db;
	global $consulta;
	?>
	<table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug"  style="border-bottom: 1px solid;">
		<tr bgcolor="#ffffff">
			<td valign="top" width="50" rowspan="2"><img src="../imagens/brasao.gif" width="45" height="45" border="0"></td>
			<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
				SIMEC- Sistema Integrado de Monitoramento Execução e Controle<br/>
				Acompanhamento da Execução Orçamentária<br/>
				MEC / SE - Secretaria Executiva <br />
				SPO - Subsecretaria de Planejamento e Orçamento
			</td>
			<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
				Impresso por: <b><?= $_SESSION['usunome'] ?></b><br/>
				Hora da Impressão: <?= date( 'd/m/Y - H:i:s' ) ?><br />
				Orçamento Fiscal e Seg.Social - EM R$ 1,00 <br />
				<? $whereAcumuladoAte = ''; ?>
				<?// $valoresFiltroAno = $consulta->pegarValoresFiltro( 'ano' ); ?>
				<? if ( count( $valoresFiltroAno ) > 0 ) : ?>
					<? $whereAcumuladoAte = " where rofano in ( '" . implode( "','", $valoresFiltroAno ) . "' ) "; ?>
				<? endif; ?>
				<?// $sqlAcumuladoAte = "select max( rofdata ) as data from financeiro.reporcfin " . $whereAcumuladoAte; ?>
				Acumulado até: <?//= formata_data( $db->pegaUm( $sqlAcumuladoAte ) ) ?>
			</td>
		</tr>
		<tr>
			<td colspan="2" align="center" valign="top" style="padding:0 0 5px 0;">
				<b><font style="font-size:14px;">Relatório de nova proposta em Ação</font></b>
			</td>
		</tr>
	</table>
	<?
}

?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Expires" content="-1">
		<title>Relatório de Responsabilidades</title>
		<style type="text/css">
			
			@media print {.notprint { display: none }}

			@media screen {
			.notscreen { display: none;  }
			.div_rolagem{ overflow-x: auto; overflow-y: auto; width:19.5cm;height:350px;}
			.topo { position: absolute; top: 0px; margin: 0; padding: 5px; position: fixed; background-color: #ffffff;}
			}

			*{margin:0; padding:0; border:none; font-size:8px;font-family:Arial;}
			.alignRight{text-align:right !important;}
			.alignCenter{ text-align:center !important;}
			.alignLeft{text-align:left !important;}
			.bold{font-weight:bold !important;}
			.italic{font-style:italic !important;}
			.noPadding{padding:0;}
			.titulo{width:52px;}
			.tituloagrup{font-size:9px;}
			.titulolinha{font-size:9px;}
			
			#tabelaTitulos tr td, #tabelaTitulos tr th{border:2px solid black;border-left:none; border-right:none;}
			#orgao{margin:3px 0 0 0;}
			#orgao tr td{border:1px solid black;border-left:none;border-right:none;font-size:11px;}
			
			div.filtro { page-break-after: always; text-align: center; }
			
			table{width:19cm;border-collapse:collapse;}
			th, td{font-weight:normal;padding:4px;vertical-align:top;}
			thead{display:table-header-group;}
			table, tr{page-break-inside:avoid;}
			a{text-decoration:none;color:#3030aa;}
			a:hover{text-decoration:underline;color:#aa3030;}
			span.topo { position: absolute; top: 3px; margin: 0; padding: 5px; position: fixed; background-color: #f0f0f0; border: 1px solid #909090; cursor:pointer; }
			span.topo:hover { background-color: #d0d0d0; }
			
		</style>
	</head>
	<body>
		<div id="aguarde" style="background-color:#ffffff;position:absolute;color:#000033;top:50%;left:30%;border:2px solid #cccccc; width:300px;">
			<center style="font-size:12px;"><br><img src="../imagens/wait.gif" border="0" align="absmiddle"> Aguarde! Gerando Relatório...<br><br></center>
		</div>
		<script type="text/javascript">
			self.focus();
		</script>
		<?php ob_flush();flush(); ?>
		<?php
		// realiza consulta, mantem dados na sessão para as imagens
//		$itens = $consulta->consultar();
//		$_SESSION['consulta_financeira'] = array();
//		$_SESSION['consulta_financeira']['itens'] = $itens['itens'];
//		$_SESSION['consulta_financeira']['agrupadores'] = $consulta->pegarAgrupadores( true );
		// FIM realiza consulta, mantem dados na sessão para as imagens
		
?>
		<table>
			<thead>
				<tr>
					<th class="noPadding" align="left">
						<? cabecalhoBrasao(); ?>
						<table id="tabelaTitulos" align="left">
							<thead>
								<tr>
									<th class="bold alignLeft" width="35%"><?= $geral ?></th>
									<th class="bold alignLeft" width="35%" style="padding: 0 25px 0 10px;">&nbsp;</th>
								</tr>
							</thead>
						</table>
					</th>
				</tr>
			</thead>
			<tbody>
				<tr>
					<td class="noPadding" align="left">
						<div class="div_rolagem">
		      
				      	<table border="1" class="tabelaDados">
							<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 0px;">
									Programa:
								</td>
								<td width="65%" style="padding: 0 15px 0 5px;">
								<b><?=$prgcod?></b> - <?=$prgdsc?>
								</td>
							</tr>

							
							<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
									Função:
								</td>
								<td width="65%" style="padding: 0 15px 0 5px;">
								<?=$fundsc?>
								</td>
							</tr>
							
							<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
									Subfunção:
								</td>
								<td width="65%" style="padding: 0 15px 0 5px;">
								<?=$sfudsc?>
								</td>
							</tr>
							
							<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
									Título:
								</td>
								<td width="65%" style="padding: 0 15px 0 5px;">
								<?=$acadsc?>
								</td>
							</tr>

							<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
									Finalidade:
								</td>
								<td width="65%" style="padding: 0 30px 0 5px;">
								<?=$acafinalidade?>
								</td>
							</tr>

							<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
									Descrição:
								</td>
								<td width="65%" align="justify" valign="middle" style="padding: 0 15px 0 0;">
								<?=$acadescricao?>
								</td>
							</tr>

							<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
									Produto (Bem ou Serviço):
								</td>
								<td width="65%" style="padding: 0 15px 0 5px;">
								<?=$prodsc?>
								</td>
							</tr>
							
							<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
									Especificação do Produto:
								</td>
								<td width="65%" style="padding: 0 15px 0 5px;">
								<?=$acadscproduto?>
								</td>
							</tr>
							
							<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
									Unidade de Medida:
								</td>
								<td width="65%" style="padding: 0 15px 0 5px;">
								<?=$unmdsc?>
								</td>
							</tr>

							<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
									Tipo de ação:
								</td>
								<td width="65%" style="padding: 0 15px 0 5px;">
								<?=$tacdsc?>
								</td>
							</tr>
							
							<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
									Esfera:
								</td>
								<td width="65%" style="padding: 0 15px 0 5px;">
								<?=$esfdsc?>
								</td>
							</tr>
							
							<tr>
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
							   Forma de Implementação:</td>
								<td width="65%" style="padding: 0 15px 0 5px;">
					                Direta <input type="checkbox" value="t" name="acasnmedireta" <?if ($acasnmedireta=='t') print '   checked'?>> &nbsp;&nbsp;&nbsp;
					            	Descentralizada <input type="checkbox" value="t" name="acasnmedesc" <?if ($acasnmedesc=='t') print '   checked'?>> &nbsp;&nbsp;&nbsp;
					                Linha de Crédito <input type="checkbox" value="t" name="acasnmelincred" <?if ($acasnmelincred=='t') print '   checked'?>> &nbsp;&nbsp;&nbsp;
						            Transferência <input type="checkbox" value="T" name="lista" <?if ($lista=='T') print '   checked'?>> &nbsp;&nbsp;&nbsp;                 
						            Obrigatória <input type="checkbox" value="t" name="acasntransfobrigatoria" <?if ($acasntransfobrigatoria=='t') print '   checked'?>> &nbsp;&nbsp;&nbsp;
						            Voluntária <input type="checkbox" value="t" name="acasntransfvoluntaria" <?if ($acasntransfvoluntaria=='t') print '   checked'?>> &nbsp;&nbsp;&nbsp;
						            Outras <input type="checkbox" value="t" name="acasntransfoutras" <?if ($acasntransfoutras=='t') print '   checked'?>> &nbsp;&nbsp;&nbsp;           
				               </td>
					         </tr>       
						    
							<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
									Detalhamento da Implementação:
								</td>
								<td width="65%" style="padding: 0 15px 0 5px;">
								<?=$acadetalhamento;?>
								</td>
							</tr>
							
							<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
									Base legal:
								</td>
								<td width="65%" style="padding: 0 15px 0 5px;">
								<?=$acabaselegal;?>
								</td>
							</tr>

							<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
									Tipo de inclusão da ação:
								</td>
								<td width="65%" style="padding: 0 15px 0 5px;">
								<?=$tindsc;?>
								</td>
							</tr>
							<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
									Custo total estimado<br>(Para ações tipo projeto ou não orçamentárias):
								</td>
								<td width="65%" style="padding: 0 15px 0 5px;">
								<?=$acavlrcustototal;?>
								</td>
							</tr>

							<tr>
							   <td align='left' width="35%" class="SubTituloDireita">Duração do Projeto<br>(Para ações tipo projeto ou não orçamentárias):</td>
								<td width="65%" style="padding: 0 15px 0 5px;">
								De <?=$acamesinicio.'/'.$acaanoinicio?> à <?=$acamestermino.'/'.$acaanotermino?>
								</td>
							   </tr>
							  <tr> 
							
							  <?
							    if ($acasnoutrasfontes =='t') {?>
								<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
									<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
										Quanto ao Orçamento:
									</td>
									<td width="65%" style="padding: 0 15px 0 5px;">
									Não orçamentária
									</td>
								</tr>
							   <? } else {?>
									<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
										<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
											Quanto ao Orçamento:
										</td>
										<td width="65%" style="padding: 0 15px 0 5px;">
										Orçamentária
										</td>
									</tr>
									<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
										<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
											Tipo de Orçamento:
										</td>
										<td width="65%" style="padding: 0 15px 0 5px;">
									    <?
									         if ($acasnfiscalseguridade=='t') print 'Fiscal&nbsp;&nbsp;&nbsp;&nbsp;';
								  	         if ($acasnfiscalseguridade=='f') print 'Seguridade&nbsp;&nbsp;&nbsp;&nbsp;';
								  	         if ($acasninvestatais=='t') print 'Investimentos Estatais&nbsp;&nbsp;&nbsp;&nbsp;';
									    ?>
										</td>
									</tr>

							      <?if ($taccod==1){?>
									<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
										<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
											Evolução da Situação Física:
										</td>
										<td width="65%" style="padding: 0 15px 0 5px;">
							             <table border=1>
							                <tr>
							                   <td width='33%'>Total</td>
							                   <td width='33%'>Realizado até <?=strval($_SESSION['exercicio'])-2?></td>
							                   <td width='33%'>Previsto em <?=$_SESSION['exercicio']-1?></td>
							                </tr>
							                <tr>
							                   <td width='33%'><?=$acaqtdcustototal?></td>
							                   <td width='33%'><?=$acaqtdateanoanterior?></td>
							                   <td width='33%'><?=$acaqtdprevistoanocorrente?></td>
							                </tr>
							             </table>
										</td>
									</tr>

				             <tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
									Justificativa da Repercussão Financeira sobre o Custeio da União:
								</td>
								<td width="65%" style="padding: 0 15px 0 5px;">
								<?=$acarepercfinanceira;?>
								</td>
							</tr>
				             <tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
									Valor Estimado da Repercussão Financeira (R$/ano):
								</td>
								<td width="65%" style="padding: 0 15px 0 5px;">
								<?=number_format($acavlrrepercfinanceira,2,',','.')?>
								</td>
							</tr>
					      <?}?>
					      <?}?>

					      <tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
									Observações:
								</td>
								<td width="65%" style="padding: 0 15px 0 5px;">
								<?=$acaobs;?>
								</td>
							</tr>


						</table>
						</div>
					</td>
				</tr>
			</tbody>
		</table>
		<script type="text/javascript" language="javascript">
			document.getElementById( 'aguarde' ).style.visibility = 'hidden';
			document.getElementById('aguarde').style.display = 'none';
		</script>
	</body>
</html>
*/