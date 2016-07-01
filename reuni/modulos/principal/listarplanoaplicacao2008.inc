<?php

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, "&unicod=" . $_SESSION['unicod'] );
$titulo_modulo='Plano Aplicação - 2008';
monta_titulo($titulo_modulo, '');

// excluindo um registro da lista de versões
if(isset($_REQUEST['evento']) && ($_REQUEST['evento'] != '')){
	if($_REQUEST['evento'] == 'E'){
		$sql = "UPDATE reuni.versaoplano  SET vplstatus = 'I' WHERE vplid = ".$_REQUEST['vplid']."";	
		$db->executar($sql);
		unset($_REQUEST['evento']);
		$db->commit();
		$db->sucesso('principal/listarplanoaplicacao2008', "&acao=C&unicod=".$_SESSION['unicod']."");
		
	}
}

?>

<!-- corpo da página -->
<?php

	$sql_40 = "select distinct	
				v.vplid as vplid, 
				to_char(v.vpldtinclusao, 'DD/MM/YYYY HH24:MI:SS') as data, 
				s.usunome as responsavel
				from reuni.lancamentodesppercent40 l
				inner join reuni.versaoplano v on v.vplid = l.vplid
				inner join seguranca.usuario s on s.usucpf = v.usucpf
				where l.unicod = '".$_SESSION['unicod']."' and v.vplstatus = 'A' and v.vplano = '2008'
				order by data
				";
	$dados = $db->carregar($sql_40);
	
	if( ! $dados){
		$sql_40 = "select distinct	
				v.vplid as vplid, 
				to_char(v.vpldtinclusao, 'DD/MM/YYYY HH24:MI:SS') as data,  
				s.usunome as responsavel
				from reuni.lancamentodesp l
				inner join reuni.versaoplano v on v.vplid = l.vplid
				inner join seguranca.usuario s on s.usucpf = v.usucpf
				where l.unicod = '".$_SESSION['unicod']."' and v.vplstatus = 'A' and v.vplano = '2008'
				order by data
				";
		$dados = $db->carregar($sql_40);
	}
	
	$mensagem = '';
	
	if( $dados ){		
				
		foreach($dados as $chave => $val){	
			  			
			if($chave == ( count($dados) -1 )){
				$acao = "<a href=\"reuni.php?modulo=principal/planoaplicacao2008&acao=C&evento=A&vplid=".$val['vplid']."&unicod=".$_SESSION['unicod']."\"><img src=\"/imagens/alterar.gif \" border=0 title=\"Editar\"></a>&nbsp;&nbsp;
				 		 <a href=\"reuni.php?modulo=principal/listarplanoaplicacao2008&acao=C&evento=E&vplid=".$val['vplid']."&unicod=".$_SESSION['unicod']."\"><img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\"></a>&nbsp;&nbsp;
				 		 <a style=\"cursor:pointer\" onclick=\"relatorioPlanoAplicacao2008(".$_SESSION['unicod'].",".$val['vplid'].")\"><img src=\"/imagens/print.png \" border=0 title=\"Imprimir\"></a>
				 		 ";
						 	
		
			}else{
				//$acao = "<a style=\"cursor:pointer\" onclick=\"relatorioPlanoAplicacao2008(".$_SESSION['unicod'].",".$val['vplid'].")\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src=\"/imagens/print.png \" border=0 title=\"Imprimir\"></a>";
				$acao = "<a ><img src=\"/imagens/alterar_01.gif \" border=0 title=\"Editar\"></a>&nbsp;&nbsp;
				 		 <a ><img src=\"/imagens/excluir_01.gif \" border=0 title=\"Excluir\"></a>&nbsp;&nbsp;
				 		 <a style=\"cursor:pointer\" onclick=\"relatorioPlanoAplicacao2008(".$_SESSION['unicod'].",".$val['vplid'].")\"><img src=\"/imagens/print.png \" border=0 title=\"Imprimir\"></a>
				 		 ";
			}	
			
			$dados_array[$chave] = array("acao" 		 => $acao, 
										 "data" 		 => $val['data'], 
										 "responsavel"   => $val['responsavel']
										 );
		}
		 $cabecalho = array("Ações", "Data", "Responsavel");
        
   		 $db->monta_lista_array($dados_array, $cabecalho, 50, 20, '', 'center', '');
   		 
    	echo("<tr>
		 <td align='center'><br/>
			<input type='button' name='inserir' value='Inserir Plano' onclick='javascript:location.href=\"reuni.php?modulo=principal/planoaplicacao2008&acao=C&evento=N&unicod={$_SESSION['unicod']}\"'/>
		</td>
		</tr>");
	}else{
		 $db->monta_lista_array("", $cabecalho, 50, 20, '', 'center', '');
	}	
?>
<script type="text/javascript">

//funcao para acionar relatorio plano de aplicação 2008
function relatorioPlanoAplicacao2008( unicod, vplid )
{
	var janela = window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/reuni/reuni.php?modulo=principal/relatorio2008&acao=C&vplid='+ vplid +'&unicod=' + unicod,'blank','height=800,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no');
	janela.focus();
}

</script>
