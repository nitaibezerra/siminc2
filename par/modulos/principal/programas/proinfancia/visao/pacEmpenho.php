<?php

require_once APPRAIZ . "emenda/classes/WSEmpenho.class.inc";
require_once APPRAIZ . "www/emenda/fndeWebservice.php";

$escrita = verificaPermissãoEscritaUsuarioPreObra($_SESSION['usucpf'], $_REQUEST['preid']);
$preid 	 = $_SESSION['par']['preid'] ? $_SESSION['par']['preid'] : $_REQUEST['preid'];

$docid = prePegarDocid($preid);
$esdid = prePegarEstadoAtual($docid);

$oSubacaoControle = new SubacaoControle();
$oPreObraControle = new PreObraControle();

$arObra = $oSubacaoControle->recuperarPreObra($preid);

?>				
<?php echo carregaAbasProInfancia("par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=empenho&preid=".$preid, $preid, $descricaoItem ); ?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="subtitulocentro" colspan="2">DADOS DA OBRA</td>		
	</tr>
	<tr>
		<td class="subtitulodireita">UF:</td>
		<td><?php echo $arObra['estuf'] ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Município:</td>
		<td><?php echo $arObra['mundescricao'] ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Tipo de Obra:</td>
		<td><?php echo $arObra['ptodescricao'] ?></td>
	</tr>
	<tr>
		<td class="subtitulocentro" colspan="2">INFORMAÇÕES PARA O ENPENHO</td>		
	</tr>
	<tr>
		<td class="subtitulodireita">Número de Processo:</td>
		<td><?php echo campo_texto('numeroprocesso', 'S', 'S', $label, 25, $max, $masc, $hid) ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">PIRES:</td>
		<td>
			<?php 
			$sql = "SELECT DISTINCT 
				    pi.pliptres as codigo,
				    pi.pliptres as descricao,
				    exf.pliid as pliid,
				    exf.exfid,
                    exf.exfvalor
				FROM
				    monitora.planointerno pi
				    left join emenda.execucaofinanceira exf
						on exf.pliid = pi.pliid
				        and exf.exfstatus = 'A'
				        --and exf.ptrid = $ptrid
				        --and exf.pedid = $pedid
				WHERE 
				    pi.plisituacao = 'S'
				   --AND acaid = ".$acaid."												 
					
				    union
					 
					SELECT DISTINCT 
					 pi.pliptres as codigo,
					 pi.pliptres as descricao,
					 exf.pliid as pliid,
					 exf.exfid,
                     exf.exfvalor
					FROM
					 monitora.planointerno pi
					 inner join emenda.execucaofinanceira exf on exf.pliid = pi.pliid
					  and exf.exfstatus = 'A'
					--WHERE exf.ptrid = $ptrid
					--and exf.pedid = $pedid
					--AND acaid = ".$acaid;
			
			$db->monta_combo('ptres', $sql, 'S', $acao, $opc, 'Selecione...');			
			?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">PI:</td>
		<td>
			<?php
			$sql = "SELECT DISTINCT 
					    pi.pliid as codigo,					    
					    pi.plicod ||' - '||pi.plititulo as descricao
					FROM 
					    monitora.planointerno pi";
			
			$db->monta_combo('numpi', $sql, 'S', $acao, $opc, 'Selecione...');			 
			?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Fonte SIAFI:</td>
		<td></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Valor a empenhar:</td>
		<td><?php echo campo_texto('valorempenhar', 'S', 'S', $label, 25, $max, '###.###.###,##', $hid) ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Número do Banco:</td>
		<td><?php echo campo_texto('numerobanco', 'S', 'S', $label, 25, $max, $masc, $hid) ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Número da Agência:</td>
		<td><?php echo campo_texto('numeroagencia', 'S', 'S', $label, 25, $max, $masc, $hid) ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Número da Conta:</td>
		<td><?php echo campo_texto('numeroconta', 'S', 'S', $label, 25, $max, $masc, $hid) ?></td>
	</tr>
	<tr>
		<td class="subtitulocentro" colspan="2"><input type="button" value="Solicitar empenho + abertura de conta" /></td>		
	</tr>
</table>
<?php 
$arDadosTemporarios = array();
array_push($arDadosTemporarios, array('nempenho' => 1,'data' => '2011-01-13 15:10:52','status' => 'Empenho Solicitado'));
array_push($arDadosTemporarios, array('nempenho' => 2,'data' => '2010-12-12 09:02:37','status' => 'Empenho Efetivado'));
array_push($arDadosTemporarios, array('nempenho' => 3,'data' => '2010-11-04 10:24:01','status' => 'Empenho Cancelado'));
?>
<br/>
<center>
<div style="width:95%;background:#f0f0f0;">
	<b>Lista de empenhos</b>
</div>
</center>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <tr>
    <th>Comando</th>
    <th>Número do empenho</th>
    <th>Data</th>
    <th>Status</th>
  </tr>
<?php $x=0; ?>
<?php foreach($arDadosTemporarios as $dados): ?>
  <?php $cor = ($x % 2) ? '#f0f0f0' : 'white'; ?>
  <tr style="background:<?php echo $cor; ?>">
    <td>
    	<img alt="" src="../imagens/alterar.gif">
    	<img alt="" src="../imagens/excluir.gif">
    </td>
    <td><?php echo $dados['nempenho'] ?></td>
    <td><?php echo $dados['data'] ?></td>
    <td><?php echo $dados['status'] ?></td>
  </tr>
  <?php $x++; ?>	
<?php endforeach; ?>
  <tr>
  	<td colspan="2">Total de registros: <?php echo count($arDadosTemporarios) ?></td>
  </tr>
</table>