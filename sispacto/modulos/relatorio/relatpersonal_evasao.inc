<?
/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

function evasaoPorUniversidade($dados) {
	global $db;
	
	echo $db->pegaUm("SELECT pfldsc FROM seguranca.perfil WHERE pflcod='".$dados['pflcod']."'");
	
	$sql = "select * FROM sispacto.universidadecadastro un 
			inner join sispacto.universidade u on u.uniid = un.uniid";
		
	$lista = $db->carregar($sql);
		
	if($lista[0]) {
		foreach($lista as $l) {
			
			$qtdtrocas = $db->pegaUm("select count(*) as qtdtrocas
										from sispacto.historicotrocausuario h
										inner join seguranca.perfil p on p.pflcod = h.pflcod
										where hstacao in('R','T') and h.uncid='".$l['uncid']."' AND p.pflcod='".$dados['pflcod']."'");
				
				
			$evasao = $db->pegaUm("SELECT count(*) as t FROM (
									SELECT i.iusd,
										i.iusnome,
										(select count(*) from sispacto.mensario m inner join sispacto.mensarioavaliacoes ma on ma.menid = m.menid where (ma.mavfrequencia=0 or ma.mavfrequencia is null) and m.fpbid=8 and m.iusd = i.iusd) as out2013,
										(select count(*) from sispacto.mensario m inner join sispacto.mensarioavaliacoes ma on ma.menid = m.menid where (ma.mavfrequencia=0 or ma.mavfrequencia is null) and m.fpbid=9 and m.iusd = i.iusd) as nov2013,
										(select count(*) from sispacto.mensario m inner join sispacto.mensarioavaliacoes ma on ma.menid = m.menid where (ma.mavfrequencia=0 or ma.mavfrequencia is null) and m.fpbid=10 and m.iusd = i.iusd) as dez2013
									FROM sispacto.identificacaousuario i
									INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd
									INNER JOIN sispacto.fatoresdeavaliacao a ON a.fatpflcodavaliado = t.pflcod
									WHERE i.iusstatus='A' and t.pflcod=".$dados['pflcod']." and i.uncid='".$l['uncid']."' AND a.fatfrequencia IS NOT NULL
								) foo
								WHERE out2013>0 AND nov2013>0 AND dez2013>0");
				
			$total = $db->pegaUm("SELECT count(*) as c FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd WHERE i.iusstatus='A' AND t.pflcod = '".$dados['pflcod']."' AND i.uncid='".$l['uncid']."'");
				
			$arrLista[] = array("universidade" => $l['unisigla']." - ".$l['uninome'],"qtdtrocas" => (($qtdtrocas)?$qtdtrocas:"0"),"naoparticipa" => (($evasao)?$evasao:"0"), "totalperfil" => $total, "porc" => (($total)?(($l['qtdtrocas']+$evasao)/$total)*100:"0") );
		}
	}
		
	$cabecalho = array("Universidade","Qtd troc/rem","Qtd não participa","Total","%");
	$db->monta_lista_simples($arrLista,$cabecalho,1000,5,'N','100%','',true, false, false, true);
	
}


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="JavaScript" src="./js/sispacto.js"></script>
<script>
function evasaoPorUniversidade(pflcod, obj) {
	if(obj.title=='mais') {
		divCarregando();
		ajaxatualizar('buscar=1&relatorio=relatpersonal_evasao.inc&requisicao=evasaoPorUniversidade&pflcod='+pflcod,'td_detalhar');
		obj.title='menos'
		obj.src='../imagens/menos.gif'
		divCarregado();
	} else {
		jQuery("#td_detalhar").html('');
		obj.title='mais'
		obj.src='../imagens/mais.gif'
	
	}
}
</script>

	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr><td class="SubTituloEsquerda">
		<fieldset>
			Qtd troc/rem - Número de cursistas que foram removidos ou substituidos pelo coordenador geral da universidade<br/>
			Qtd não participa - Número de cusistas que participam do cuso, porém não possuem frequência nos meses de Out/2013, Nov2013 e Dez2013.<br>
			% - Porcentagem dos bolsistas evadidos (((Qtd troc/rem + Qtd não participa)  / Total) *100)  
		</fieldset>
		</td></tr>		
        <tr><td class="SubTituloEsquerda">Evasão por perfil</td></tr>

		<tr>
			<td><?
			$sql = "select p.pflcod, p.pfldsc, count(*) as qtdtrocas
					from sispacto.historicotrocausuario h 
					inner join seguranca.perfil p on p.pflcod = h.pflcod
					where hstacao in('R','T')
					group by p.pfldsc, p.pflcod 
					order by 3 desc";
			
			$lista = $db->carregar($sql);
			
			if($lista[0]) {
				foreach($lista as $l) {
					
					$evasao = $db->pegaUm("SELECT count(*) as t FROM (
									SELECT i.iusd,
										i.iusnome,
										(select count(*) from sispacto.mensario m inner join sispacto.mensarioavaliacoes ma on ma.menid = m.menid where (ma.mavfrequencia=0 or ma.mavfrequencia is null) and m.fpbid=8 and m.iusd = i.iusd) as out2013,
										(select count(*) from sispacto.mensario m inner join sispacto.mensarioavaliacoes ma on ma.menid = m.menid where (ma.mavfrequencia=0 or ma.mavfrequencia is null) and m.fpbid=9 and m.iusd = i.iusd) as nov2013,
										(select count(*) from sispacto.mensario m inner join sispacto.mensarioavaliacoes ma on ma.menid = m.menid where (ma.mavfrequencia=0 or ma.mavfrequencia is null) and m.fpbid=10 and m.iusd = i.iusd) as dez2013
									FROM sispacto.identificacaousuario i 
									INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
									INNER JOIN sispacto.fatoresdeavaliacao a ON a.fatpflcodavaliado = t.pflcod 
									WHERE i.iusstatus='A' and t.pflcod=".$l['pflcod']." AND a.fatfrequencia IS NOT NULL
								) foo 
								WHERE out2013>0 AND nov2013>0 AND dez2013>0");
					
					$total = $db->pegaUm("SELECT count(*) as c FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd WHERE i.iusstatus='A' AND t.pflcod = '".$l['pflcod']."'");
					
					$arrLista[] = array("acao" => "<img src=../imagens/mais.gif title=mais onclick=\"evasaoPorUniversidade('".$l['pflcod']."', this);\">","pfldsc" => $l['pfldsc'],"qtdtrocas" => $l['qtdtrocas'],"naoparticipa" => (($evasao)?$evasao:"0"), "totalperfil" => $total, "porc" => (($l['qtdtrocas']+$evasao)/$total)*100);
				}
			}
			
			$cabecalho = array("&nbsp;","Perfil","Qtd troc/rem","Qtd não participa","Total","%");
			$db->monta_lista_simples($arrLista,$cabecalho,1000,5,'N','100%','',true, false, false, true);
			
			?></td>
		</tr>
		
        <tr><td class="SubTituloEsquerda" id="td_detalhar"></td></tr>
	</table>