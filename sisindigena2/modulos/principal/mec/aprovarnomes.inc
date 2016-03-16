<?
$goto_ant = 'sisindigena2.php?modulo=principal/mec/mec&acao=A&aba=gerenciarturmas';
$goto_pro = 'sisindigena2.php?modulo=principal/mec/mec&acao=A&aba=reenviarpagamentos';
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


$sql = "SELECT DISTINCT i.iusd, 
						replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') as iuscpf, 
						i.iusnome, 
						p.pflcod,
						p.pfldsc, 
						u.uninome as universidade, 
						s.logresponse,
						u2.uninome as nucleo 
						

		from sisindigena2.identificacaousuario i 
		inner join sisindigena2.tipoperfil t on t.iusd = i.iusd 
		inner join seguranca.perfil p on p.pflcod = t.pflcod 
		left join sisindigena2.universidadecadastro c on c.uncid = i.uncid 
		left join sisindigena2.universidade u on u.uniid = c.uniid 
		left join sisindigena2.nucleouniversidade pp on pp.picid = i.picid 
		left join sisindigena2.universidade u2 on u2.uniid = pp.uniid 
		inner join log_historico.logsgb_sisindigena2 s on s.logcpf = i.iuscpf and s.logservico='gravarDadosBolsista' and s.logerro=true
		where iustermocompromisso=true and logresponse ilike '%Erro: 00026:%' and cadastradosgb!=true ORDER BY i.iusnome;";
$identificacaousuario = $db->carregar($sql);

if($identificacaousuario[0]) {
	foreach($identificacaousuario as $ius) {
		
		$sl = explode("(",$ius['logresponse']);
		$sl = explode(")",$sl[1]);
		
		if(substr(strtoupper($ius['iusnome']),0,9)!=substr(strtoupper(trim($sl[0])),0,9)) {
			if($consulta) {
				$arrListaP[$ius['iuscpf']] = array("",$ius['iuscpf'],$ius['iusnome'],$sl[0]."<input type=hidden name=\"nome_receita[".trim($ius['iuscpf'])."]\" value=\"".$sl[0]."\">",$ius['pfldsc'],$ius['uninome'],$ius['esfera']);				
			} else {
				$arrListaP[$ius['iuscpf']] = array("<input type=\"checkbox\" name=\"cpf[]\" value=\"".trim($ius['iuscpf'])."\"> <img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluirUsuarioPerfil(".$ius['pflcod'].",".$ius['iusd'].");\">",$ius['iuscpf'],$ius['iusnome'],$sl[0]."<input type=hidden name=\"nome_receita[".trim($ius['iuscpf'])."]\" value=\"".$sl[0]."\">",$ius['pfldsc'],$ius['universidade'],$ius['nucleo']);				
			}
			
		}
		
	}
	
	if($arrListaP) {
		foreach($arrListaP as $arr) {
			$arrLista[] = $arr;
		}
	}
	
	
}

if(!$arrLista) $arrLista = array();

$cabecalho = array("&nbsp;","CPF","Nome atual","Nome Receita Federal","Perfil","Universidade","Núcleo");
$db->monta_lista($arrLista,$cabecalho,10000,5,'N','center','N',"formaprovarnomes");

?>
	
	<p align="center"><input type="button" value="Aprovar Troca" name="aprovar"  onclick="aprovarNomesSGB();"></p>	
	</td>
</tr>


<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td>
	<input type="button" value="Anterior" onclick="divCarregando();window.location='<?=$goto_ant ?>';">
	<input type="button" value="Próximo" onclick="divCarregando();window.location='<?=$goto_pro ?>';">
	</td>
</tr>


</table>
