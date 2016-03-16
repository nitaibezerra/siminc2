<?

include_once "_funcoes_pagamentos.php";


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/escolaterra.js"></script>';

echo "<br>";


monta_titulo( "Efetuando Pagamentos", "Lista de envio bolsas para o Sistema de Gestão de Bolsas(SGB) - FNDE");

?>
<script>
function aprovarNomesSGB() {

	var conf = confirm('Deseja realmente aprovar a troca do nomes selecionados?');
	if(conf) {
		var input = document.createElement("input");
		input.setAttribute("type", "hidden");
		input.setAttribute("name", "requisicao");
		input.setAttribute("value", "aprovarTrocaNomesSGB");
		document.getElementById("formaprovarnomes").appendChild(input);
		document.getElementById("formaprovarnomes").submit();
	}

}

function excluirUsuarioPerfil(pflcod,iusd) {
	var conf = confirm('Deseja realmente excluir o este cargo? \n\n- Essa ação será definitiva e não poderá ser incluído um novo membro.\n- Caso este ja tenha recebido algum pagamento, o sistema não permitirá a exclusão\n- Ícone ao lado é uma ferramenta de substituição, tenha certeza de que ela não é a ferramenta que esta necessitando.');
	if(conf) {
		window.location='<?=$_SERVER['REQUEST_URI'] ?>&requisicao=excluirUsuarioPerfil&pflcod='+pflcod+'&iusd='+iusd;
	}
}
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita">Orientações</td>
	<td colspan="2">
	Os usuários que estão com os nomes desatualizados com os da Receita Federal e possuem os 9(nove) primeiros dígitos diferentes com o nome da receita federal.
	</td>
</tr>
<tr>
	<td colspan="3">
	
<?

$consulta = verificaPermissao();


$sql = "SELECT DISTINCT i.iusid, 
						replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') as iuscpf, 
						i.iusnome, 
						p.pflcod,
						p.pfldsc, 
						s.logresponse, 
						c.estuf

		from escolaterra.identificacaousuario i 
		inner join escolaterra.tipoperfil t on t.iusid = i.iusid 
		inner join seguranca.perfil p on p.pflcod = t.pflcod 
		left join escolaterra.ufparticipantes c on c.ufpid = i.ufpid 
		inner join (select max(logid) as logid, logcpf from log_historico.logsgb_escolaterra s where s.logservico='gravarDadosBolsista' and s.logerro=true group by logcpf) foo on foo.logcpf = i.iuscpf
		inner join log_historico.logsgb_escolaterra s on s.logid = foo.logid
		where cadastradosgb=false and iustermocompromisso=true and logresponse ilike '%Erro: 00026:%' and cadastradosgb!=true ORDER BY i.iusnome;";
$identificacaousuario = $db->carregar($sql);

if($identificacaousuario[0]) {
	foreach($identificacaousuario as $ius) {
		
		$sl = explode("(",$ius['logresponse']);
		$sl = explode(")",$sl[1]);
		
		if(substr(strtoupper($ius['iusnome']),0,9)!=substr(strtoupper(trim($sl[0])),0,9)) {
			if($consulta) {
				$arrListaP[$ius['iuscpf']] = array("",$ius['iuscpf'],$ius['iusnome'],$sl[0]."<input type=hidden name=\"nome_receita[".trim($ius['iuscpf'])."]\" value=\"".$sl[0]."\">",$ius['pfldsc'],$ius['estuf']);				
			} else {
				$arrListaP[$ius['iuscpf']] = array("<input type=\"checkbox\" name=\"cpf[]\" value=\"".trim($ius['iuscpf'])."\"> <img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluirUsuarioPerfil(".$ius['pflcod'].",".$ius['iusd'].");\">",$ius['iuscpf'],$ius['iusnome'],$sl[0]."<input type=hidden name=\"nome_receita[".trim($ius['iuscpf'])."]\" value=\"".$sl[0]."\">",$ius['pfldsc'],$ius['estuf']);				
			}
			
		}
		
	}
	
	if($arrListaP) {
		foreach($arrListaP as $arr) {
			$arrLista[] = $arr;
		}
	}
	
	
}

if(!$arrLista[0]) $arrLista = array(); 

$cabecalho = array("&nbsp;","CPF","Nome atual","Nome Receita Federal","Perfil","Vínculo");
$db->monta_lista($arrLista,$cabecalho,10000,5,'N','center','N',"formaprovarnomes");

?>
	
	<p align="center"><input type="button" value="Aprovar Troca" name="aprovar"  onclick="aprovarNomesSGB();"></p>	
	</td>
</tr>

</table>
