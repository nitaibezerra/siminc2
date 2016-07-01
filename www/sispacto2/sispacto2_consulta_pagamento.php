<?
$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */


// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/workflow.php";

error_reporting(1);

include "_constantes.php";
include "_funcoes.php";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf']) {
	$_SESSION['usucpforigem'] = '';
	$_SESSION['usucpf'] = '';
}

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

?>
<html>
<head>
	<title>SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação</title>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	<script>
	function consultarPagamento() {
		if(document.getElementById('cpf').value=='') {
			alert('CPF em branco');
			return false;
		}
		
		if(!validar_cpf(document.getElementById('cpf').value)) {
			alert('CPF inválido');
			return false;
		}
		
		if(document.getElementById('datanascimento').value=='') {
			alert('Data de Nascimento em branco');
			return false;
		}
		
		if(!validaData(document.getElementById('datanascimento'))) {
			alert('Data de Nascimento inválida');
			return false;
		}
		
		document.getElementById('formulario').submit();
	
	}
	
	function abrirDetalhes(id) {
		if(document.getElementById('img_'+id).title=='mais') {
			document.getElementById('tr_'+id).style.display='';
			document.getElementById('img_'+id).title='menos';
			document.getElementById('img_'+id).src='../imagens/menos.gif'
		} else {
			document.getElementById('tr_'+id).style.display='none';
			document.getElementById('img_'+id).title='mais';
			document.getElementById('img_'+id).src='../imagens/mais.gif'

		}
	
	}
	</script>
</head>
<body topmargin="0" leftmargin="0">

<?

$menu = array( 0 => array("id" => 1, "descricao" => "SISPACTO 2013", "link" => "/sispacto/sispacto_consulta_pagamento.php"),
			   1 => array("id" => 2, "descricao" => "SISPACTO 2014", "link" => "/sispacto2/sispacto2_consulta_pagamento.php"));

echo "<br/>";
echo montarAbasArray($menu, $abaAtiva);


?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td width="25%"><img src="/includes/layout/azul/img/logo.png" border="0" /></td>
	<td valign="middle" style="font-size:15px;"><b>Consulta Pagamento/Avaliação no SISPACTO 2014</b></td>
</tr>
</table>
<?php

if($_POST['requisicao']=='consultarPagamento') :
	
	if(strlen(trim($_POST['datanascimento']))==10 && strlen(trim($_POST['cpf']))==14) {
		$sql = "SELECT uncid, iusd FROM sispacto2.identificacaousuario WHERE iuscpf='".addslashes(str_replace(array(".","-"),array("",""),$_POST['cpf']))."' AND iusdatanascimento='".formata_data_sql($_POST['datanascimento'])."'";
		$identificacaousuario = $db->pegaLinha($sql);
	}
	
	if(!$identificacaousuario['iusd']) {
		$al = array("alert"=>"Usuário não encontrado no SISPACTO","location"=>"sispacto2_consulta_pagamento.php");
		alertlocation($al);
	}
	
	if($identificacaousuario['uncid']) {
		
		echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="5" cellPadding="8" align="center">';
		echo '<tr>';
		echo '<td class="SubTituloDireita">Recomendações:</td>';
		echo '<td>';
		
		$sql = "SELECT i2.iusnome||' ( '||p.pfldsc||' )' as iusnome,
			       CASE WHEN ma.mavrecomendadocertificacao='1' THEN '<font style=color:blue;><b>Recomendado '|| CASE WHEN (t2.pflcod='".PFL_FORMADORIES."' OR t2.pflcod='".PFL_ORIENTADORESTUDO."') THEN 'para certificação IES' WHEN t2.pflcod='".PFL_COORDENADORLOCAL."' THEN 'para SISPACTO 2015' END ||'</b></font>'
				    	WHEN ma.mavrecomendadocertificacao='2' THEN '<font style=color:red;><b>Não recomendado '|| CASE WHEN (t2.pflcod='".PFL_FORMADORIES."' OR t2.pflcod='".PFL_ORIENTADORESTUDO."') THEN 'para certificação IES' WHEN t2.pflcod='".PFL_COORDENADORLOCAL."' THEN 'para SISPACTO 2015' END ||'</b></font>' END as certificacao,
				   '<textarea cols=\"45\" rows=\"3\" style=\"width:98%;\" class=\"txareanormal\">'||ma.mavrecomendadocertificacaojustificativa||'</textarea>'as justificativa
			FROM sispacto2.identificacaousuario i
			INNER JOIN sispacto2.mensario m ON m.iusd = i.iusd
			INNER JOIN sispacto2.mensarioavaliacoes ma ON ma.menid = m.menid
			INNER JOIN sispacto2.identificacaousuario i2 ON i2.iusd = ma.iusdavaliador
			INNER JOIN sispacto2.tipoperfil t2 ON t2.iusd = i2.iusd
			INNER JOIN seguranca.perfil p ON p.pflcod = t2.pflcod
			WHERE i.iusd='".$identificacaousuario['iusd']."' AND ma.mavrecomendadocertificacao IS NOT NULL";
		
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',false, false, false, false);
			
		echo '</td>';
		echo '</tr>';
		echo '</table>';
		
		$sql = "SELECT p.plpmaximobolsas
			FROM sispacto2.identificacaousuario i
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd
			INNER JOIN sispacto2.pagamentoperfil p ON p.pflcod = t.pflcod
			WHERE i.iusd='".$identificacaousuario['iusd']."'";
		
		$nmaximobolsas = $db->pegaUm($sql);
	
		
		$sql = "SELECT f.fpbid as codigo, 
					   rf.rfuparcela ||'º Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as descricao,
					   COALESCE((
					   	SELECT e.esddsc || ' ( ' || to_char(h.htddata,'dd/mm/YYYY HH24:MI') || ' )' as s FROM sispacto2.pagamentobolsista p 
						INNER JOIN workflow.documento d ON d.docid = p.docid 
						INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
						LEFT JOIN workflow.historicodocumento h ON h.hstid = d.hstid 
						WHERE p.iusd='".$identificacaousuario['iusd']."' AND p.fpbid=f.fpbid
						),'') as statuspagamento  
				FROM sispacto2.folhapagamento f 
				INNER JOIN sispacto2.folhapagamentouniversidade rf ON rf.fpbid = f.fpbid AND rf.pflcod=(SELECT pflcod FROM sispacto2.tipoperfil WHERE iusd=".$identificacaousuario['iusd'].")
				INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
				WHERE f.fpbstatus='A' AND rf.uncid='".$identificacaousuario['uncid']."' AND to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'01')::date,'YYYYmm') ".(($nmaximobolsas)?"LIMIT ".$nmaximobolsas:"");
		
		$folhapagamento = $db->carregar($sql);
		
		if($folhapagamento[0]) {
			echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="5" cellPadding="8" align="center">';
			echo '<tr><td class="SubTituloEsquerda" colspan="2">Extrato de pagamento/avaliações</td></tr>';
			echo '<tr><td class="SubTituloCentro" width="50%">Parcela</td><td class="SubTituloCentro" width="50%">Situação pagamento(Data de atualização)</td></tr>';
			foreach($folhapagamento as $fl) {
				
				$periodoslistados[] = $fl['codigo'];
				
				echo '<tr><td class="SubTituloEsquerda">'.$fl['descricao'].'</td>
						  <td><font size=3><b>'.$fl['statuspagamento'].'</b></font></td></tr>';
				echo '<tr><td colspan="2"><img src="../imagens/mais.gif" style="cursor:pointer;" title="mais" id="img_ava_'.$fl['codigo'].'" onclick="abrirDetalhes(\'ava_'.$fl['codigo'].'\');"> Detalhes da avaliação</td></td></tr>';
				echo '<tr style="display:none;" id="tr_ava_'.$fl['codigo'].'"><td colspan="2">';
				echo '<p align="center"><b>INFORMAÇÕES SOBRE AVALIAÇÕES</b></p>';
				$sql = "SELECT * FROM sispacto2.mensario WHERE fpbid='".$fl['codigo']."' AND iusd='".$identificacaousuario['iusd']."'";
				$mensario = $db->pegaLinha($sql);
				
				if($mensario['menid']) consultarDetalhesAvaliacoes(array('menid'=>$mensario['menid']));
				else echo '<p align=center style=color:red;>Não existem avaliações nesse período de referência</p>';
				
				echo '</td></tr>';
				echo '<tr><td colspan="2"><img src="../imagens/mais.gif" style="cursor:pointer;" title="mais" id="img_pag_'.$fl['codigo'].'" onclick="abrirDetalhes(\'pag_'.$fl['codigo'].'\');"> Detalhes do pagamento</td></td></tr>';
				echo '<tr style="display:none;" id="tr_pag_'.$fl['codigo'].'"><td colspan="2">';
				echo '<p align="center"><b>INFORMAÇÕES SOBRE PAGAMENTO</b></p>';
				$sql = "SELECT pboid FROM sispacto2.pagamentobolsista WHERE fpbid='".$fl['codigo']."' AND iusd='".$identificacaousuario['iusd']."'";
				$pboid = $db->pegaUm($sql);
				
				if($pboid) {
					consultarDetalhesPagamento(array('pboid'=>$pboid));
				} else {
					echo "<p align=center style=color:red;>Não existem pagamentos nesse período de referência</p>";	
					
					$restricao = pegarRestricaoPagamento(array('iusd' => $identificacaousuario['iusd'], 'fpbid' => $fl['codigo']));
					echo "<table class=\"listagem\" bgcolor=\"#f5f5f5\" cellSpacing=\"5\" cellPadding=\"10\" align=\"center\">";
					echo "<tr>";
					echo "<td class=\"SubTituloDireita\"><b>Possível restrição:</b></td>";
					echo "<td><b>".$restricao."</b></td>";
					echo "</tr>";
					echo "</table>";
				} 
				
				
				echo '</td></tr>';
				
			}
			
			
			if($periodoslistados) {
					
				$sql = "SELECT f.fpbid as codigo,
					   	   'Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as descricao,
						   COALESCE((
											   	SELECT e.esddsc || ' ( ' || to_char(h.htddata,'dd/mm/YYYY HH24:MI') || ' )' as s FROM sispacto2.pagamentobolsista p
												INNER JOIN workflow.documento d ON d.docid = p.docid
												INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid
												LEFT JOIN workflow.historicodocumento h ON h.hstid = d.hstid
												WHERE p.iusd='".$identificacaousuario['iusd']."' AND p.fpbid=f.fpbid
												),'') as statuspagamento
					FROM sispacto2.pagamentobolsista p
					INNER JOIN sispacto2.folhapagamento f ON f.fpbid = p.fpbid
					INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
					WHERE iusd='".$identificacaousuario['iusd']."' AND f.fpbid NOT IN('".implode("','",$periodoslistados)."')";
					
				$pagamentosrestantes = $db->carregar($sql);
					
			}
			
			if($pagamentosrestantes[0]) {
				echo '<tr><td class="SubTituloEsquerda" style="color:red;" colspan="2">Extrato de pagamento/avaliações - OUTRAS UNIVERSIDADES</td></tr>';
				echo '<tr><td class="SubTituloCentro" width="50%">Parcela</td><td class="SubTituloCentro" width="50%">Situação pagamento (Data de atualização)</td></tr>';
					
				foreach($pagamentosrestantes as $pr) {
			
					echo '<tr><td class="SubTituloEsquerda">'.$pr['descricao'].'</td>
									  <td><font size=3><b>'.$pr['statuspagamento'].'</b></font></td></tr>';
					echo '<tr><td colspan="2"><img src="../imagens/mais.gif" style="cursor:pointer;" title="mais" id="img_ava_'.$pr['codigo'].'" onclick="abrirDetalhes(\'ava_'.$pr['codigo'].'\');"> Detalhes da avaliação</td></td></tr>';
					echo '<tr style="display:none;" id="tr_ava_'.$pr['codigo'].'"><td colspan="2">';
					echo '<p align="center"><b>INFORMAÇÕES SOBRE AVALIAÇÕES</b></p>';
						
					$sql = "SELECT * FROM sispacto2.mensario WHERE fpbid='".$pr['codigo']."' AND iusd='".$identificacaousuario['iusd']."'";
					$mensario = $db->pegaLinha($sql);
						
					if($mensario['menid']) consultarDetalhesAvaliacoes(array('menid'=>$mensario['menid']));
					else echo '<p align=center style=color:red;>Não existem avaliações nesse período de referência</p>';
						
					echo '</td></tr>';
					echo '<tr><td colspan="2"><img src="../imagens/mais.gif" style="cursor:pointer;" title="mais" id="img_pag_'.$pr['codigo'].'" onclick="abrirDetalhes(\'pag_'.$pr['codigo'].'\');"> Detalhes do pagamento</td></td></tr>';
					echo '<tr style="display:none;" id="tr_pag_'.$pr['codigo'].'"><td colspan="2">';
					echo '<p align="center"><b>INFORMAÇÕES SOBRE PAGAMENTO</b></p>';
						
					$sql = "SELECT pboid FROM sispacto2.pagamentobolsista WHERE fpbid='".$pr['codigo']."' AND iusd='".$identificacaousuario['iusd']."'";
					$pboid = $db->pegaUm($sql);
						
					if($pboid) {
						consultarDetalhesPagamento(array('pboid'=>$pboid));
					} else {
						echo "<p align=center style=color:red;>Não existem pagamentos nesse período de referência</p>";
			
						$restricao = pegarRestricaoPagamento(array('iusd' => $identificacaousuario['iusd'], 'fpbid' => $pr['codigo']));
			
						echo "<table class=\"listagem\" bgcolor=\"#f5f5f5\" cellSpacing=\"5\" cellPadding=\"10\" align=\"center\">";
						echo "<tr>";
						echo "<td class=\"SubTituloDireita\"><b>Possível restrição:</b></td>";
						echo "<td><b>".$restricao."</b></td>";
						echo "</tr>";
						echo "</table>";
					}
						
						
					echo '</td></tr>';
			
				}
			
			}
			
			echo '<tr><td class="SubTituloCentro" colspan="2"><input type=button value=Voltar onclick="window.location=\'sispacto2_consulta_pagamento.php\';"></td></tr>';
			echo '</table>';
			echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="5" cellPadding="1" align="center">';
			echo '<tr><td colspan="2" style="font-size:xx-small;"><p>Prezado bolsista, após ser APROVADO no fluxo de avaliação, o pagamento das bolsas no âmbito do Pacto Nacional pela Alfabetização na Idade Certa obedece ao seguinte fluxo:</p></td></tr>';
			echo '<tr><td class="SubTituloCentro" style="font-size:xx-small;">Status de pagamento</td><td class="SubTituloCentro" style="font-size:xx-small;">Descrição</td></tr>';
			echo '<tr><td style="font-size:xx-small;">Aguardando autorização IES</td><td style="font-size:xx-small;">O bolsista foi avaliado e considerado apto a receber a bolsa. A liberação do pagamento está aguardando autorização final pela Universidade responsável pela formação.</td></tr>';
			echo '<tr><td style="font-size:xx-small;">Autorizado IES</td><td style="font-size:xx-small;">O pagamento da bolsa foi autorizado pela Universidade e está sendo processado pelos sistemas do MEC.</td></tr>';
			echo '<tr><td style="font-size:xx-small;">Aguardando autorização SGB</td><td style="font-size:xx-small;">O pagamento da bolsa está no Sistema de Gestão de Bolsas, aguardando autorização do MEC.</td></tr>';
			echo '<tr><td style="font-size:xx-small;">Aguardando pagamento</td><td style="font-size:xx-small;">O pagamento da bolsa foi autorizado pelo SGB e está em processamento.</td></tr>';
			echo '<tr><td style="font-size:xx-small;">Enviado ao Banco</td><td style="font-size:xx-small;">A ordem bancária referente ao pagamento da bolsa foi emitida. O pagamento estará disponível para saque em até 02 dias úteis, em função do processamento da O.B. pelo banco</td></tr>';
			echo '<tr><td style="font-size:xx-small;">Pagamento efetivado</td><td style="font-size:xx-small;">O pagamento foi creditado em conta e confirmado pelo banco.</td></tr>';
			echo '<tr><td style="font-size:xx-small;">Pagamento não autorizado FNDE</td><td style="font-size:xx-small;">O pagamento da bolsa não foi autorizado pelo FNDE, pois o bolsista recebe bolsa de outro programa do MEC.</td></tr>';
			echo '<tr><td style="font-size:xx-small;">Pagamento recusado</td><td style="font-size:xx-small;">Pagamento recusado em função de algum erro de registro. Será reencaminhado a IES responsável pela formação.</td></tr>';
			echo '<tr><td colspan="2" style="font-size:xx-small;"><p><b>Observação: Caso o seu status no fluxo de pagamento esteja em BRANCO, significa que o mês de referencia ainda não teve o seu fluxo de avaliação concluído. Você deve procurar a coordenação local do PACTO ou a IES responsável pela formação do seu município.</b></p></td></tr>';
			echo '</table>';
			
			
		} else {
			$al = array("alert"=>"A universidade do Usuário não possui período de referência atribuído","location"=>"sispacto2_consulta_pagamento.php");
			alertlocation($al);
		}
		
		
	} else {
		$al = array("alert"=>"Usuário não esta vinculado a nenhuma Universidade","location"=>"sispacto2_consulta_pagamento.php");
		alertlocation($al);
	}
	
else :

?>
<form method="post" id="formulario" name="formulario">
<input type="hidden" name="requisicao" value="consultarPagamento">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloDireita">CPF:</td>
	<td><input type="text" id="cpf" name="cpf" value="" size="20" onkeypress="return controlar_foco_cpf( event );" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" /></td>
</tr>
<tr>
	<td class="SubTituloDireita">Data de Nascimento:</td>
	<td><input type="text" id="datanascimento" name="datanascimento" value="" size="12" onkeyup="this.value=mascaraglobal('##/##/####',this.value);" /></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" name="consultar" value="Consultar" onclick="consultarPagamento();"></td>
</tr>
</table>
</form>
<?
endif;
?>
</body>
</html>