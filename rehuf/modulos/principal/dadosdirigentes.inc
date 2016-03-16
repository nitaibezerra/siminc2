<?php
/* Tratamento de segurança */
if(validaVariaveisSistema()) {
	echo "<script>
			alert('Problemas nas variáveis do sistema.');
			window.location='?modulo=inicio&acao=A';
		  </script>";
	exit;
}
$permissoes = verificaPerfilRehuf();
validaAcessoHospital($permissoes['verhospitais'], $_SESSION['rehuf_var']['entid']);
/* FIM - Tratamento de segurança */

/* Imprimindo cabeçalho */
include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';
/* FIM - Imprimindo cabeçalho */

/* montando o menu */
echo montarAbasArray(carregardadosmenurehuf(), "/rehuf/rehuf.php?modulo=principal/dadosdirigentes&acao=A");

/* montando cabeçalho */
monta_titulo( "REHUF", "Programa Nacional de Re-estruturação dos Hospitais Universitários Federais");
monta_cabecalho_rehuf($_SESSION['rehuf_var']['entid']);
?>
<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>
<tr><td colspan='4' class="SubTituloEsquerda"><strong>Dados dos dirigentes</strong></td></tr>
<tr>
<td class="SubTituloCentro">Cargo</td>
<td class="SubTituloCentro">Nome</td>
<td class="SubTituloCentro">Data de modificação</td>
<td class="SubTituloCentro">Usuário que modificou</td>
</tr>
<?
// selecionando todas as funções previstas para os hospitais (_constantes.php) e agregando com os dirigente cadastrados
$sql = "SELECT fun.funid, fun.fundsc, ent.entnome, ent.entid FROM entidade.funcao fun 
		LEFT JOIN entidade.funcaoentidade fen ON fen.funid = fun.funid AND fen.entid = (SELECT fen2.entid FROM entidade.funentassoc fea2
									        											LEFT JOIN entidade.funcaoentidade fen2 on fea2.fueid = fen2.fueid 
																						WHERE fea2.entid='".$_SESSION['rehuf_var']['entid']."' AND fun.funid = fen2.funid)
		LEFT JOIN entidade.entidade ent ON fen.entid = ent.entid 
		WHERE fun.funid IN('".implode("','",$_funcoes)."')";
$funcoeshospit = $db->carregar($sql);
if($funcoeshospit[0]) {
	foreach($funcoeshospit as $funent) {
		echo "<tr>";
		echo "<td class='SubTituloDireita'>". $funent['fundsc'] ." :</td>";
		echo "<td><a href=\"javascript:void(0);\" onclick=\"window.open('?modulo=principal/editardirigente&acao=A&funid=".$funent['funid']."','Dirigentes','scrollbars=yes,height=600,width=700,status=no,toolbar=no,menubar=no,location=no');\">". (($funent['entnome'])?"<img src='../imagens/alterar.gif' border='0'> ".$funent['entnome']:"<img src='../imagens/gif_inclui.gif' border='0'> Não cadastrado") ."</td>";
		
		unset($log_dirigente);
		if($funent['entid']) {
			$log_dirigente = $db->pegaLinha("SELECT to_char(h.hstdata,'dd/mm/YYYY HH24:MI') as data, u.usunome FROM entidade.historico h 
											 INNER JOIN seguranca.usuario u ON u.usucpf = h.usucpf
											 WHERE h.entid='".$funent['entid']."' AND 
											 	   h.hsturl='/rehuf/rehuf.php?modulo=principal/editardirigente&acao=A&requisicao=salvarRegistroDirigente' AND 
											  	   h.endid IS NULL 
											 ORDER BY h.hstdata DESC LIMIT 1");
		}
		
		echo "<td>".(($log_dirigente['data'])?$log_dirigente['data']:"-")."</td>";
		echo "<td>".(($log_dirigente['usunome'])?$log_dirigente['usunome']:"-")."</td>";
		
		echo "</tr>";
	}
} else {
	echo "<tr><td class='SubTituloEsquerda' colspan='2'>Problemas nas funções dos hospitais (_constantes.php)</td></tr>";
}
?>
</table>