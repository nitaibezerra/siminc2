<?php
// Exclui ação
if ( ($_REQUEST['action'] == 'delete') && !empty($_REQUEST['id'])){
	
    $acaid = $_REQUEST['id'];
    
	//Verifica a existência de vinculos
	$sql = "SELECT SUM(total) as total
			FROM(
				SELECT count(0) as total FROM painel.acaosecretaria WHERE acaid = {$acaid}
				UNION ALL
				SELECT count(0) as total FROM painel.indicador WHERE indstatus = 'A' AND acaid = {$acaid}
				UNION ALL
				SELECT count(0) as total FROM pde.ae_acaoxdesafio WHERE acaid = {$acaid}
				UNION ALL
				SELECT count(0) as total FROM pde.ae_acaoxmetapne WHERE acaid = {$acaid}
				UNION ALL
				SELECT count(0) as total FROM pde.ae_acaoxmetappa WHERE acaid = {$acaid}
				UNION ALL
				SELECT count(0) as total FROM monitora.pi_acaoestrategicasubacao WHERE acaid = {$acaid}
				UNION ALL
				SELECT count(0) as total FROM planacomorc.vinculacaoacaoestrategica WHERE acaid = {$acaid}
			) AS foo";
	$valor = $db->pegaum($sql);
	
	if ($valor > 0){
		echo "<script>
				alert('A ação estratégica não pode ser excluída, pois possui vínculos. Remova-os e tente novamente!');		  
				</script>";
	}
	else {
            // Exclui ação
            $sql = "UPDATE painel.acao set acastatus='I' WHERE acaid = {$acaid}";
            $db->executar($sql);
            $db->commit();
	
	echo "<script>
                alert('Ação estratégica excluída com sucesso!');
              </script>";
	}
}

include  APPRAIZ."includes/cabecalho.inc";

echo "<br>";

if(isset($_SESSION['acaid'])){
    unset($_SESSION['acaid']);
}

$menu = array();
$menu[0] = array("descricao" => "Pesquisa de ações estratégicas", "link"=> "/painel/painel.php?modulo=sistema/tabelaapoio/cadastroacoes/listar_acao&acao=A");
$menu[1] = array("descricao" => "Cadastro da ação estratégica", "link"=> "/painel/painel.php?modulo=sistema/tabelaapoio/cadastroacoes/formulario_acao&acao=A");
echo montarAbasArray($menu, "/painel/painel.php?modulo=sistema/tabelaapoio/cadastroacoes/listar_acao&acao=A");

monta_titulo( $titulo_modulo , '');
?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario" action="/painel/painel.php?modulo=sistema/tabelaapoio/cadastroacoes/listar_acao&acao=A">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td align='right' class="SubTituloDireita">Ação estratégica:</td>
    <td><? 
    	echo campo_texto('sbacao','N','S','',100,255,'',''); 
//    	echo "<input type='text' name='sbatitulo'>";
    	?>
    </td>
</tr>
<tr bgcolor="#cccccc">
    <td></td>
        <td>
            <input type="button" class="botao" name="bta" value="Pesquisar" onclick="submeter();">
        </td>
    </tr>
</table>
</form>
<?
//if($_POST){ 
 if(!empty($_REQUEST['sbacao'])){
	$where = " WHERE ac.acastatus = 'A' and upper(ac.acadsc) like upper('%$_REQUEST[sbacao]%') ";
 }	
 else {
	$where = "where ac.acastatus = 'A'";
 }
	$sql = "SELECT '<center>
						<a style=\"cursor:pointer;\" onclick=\"alteraracao(\''||ac.acaid||'\');\">
							<img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
						</a>
						<a  style=\"cursor:pointer;\" onclick=\"removeracao(\''||ac.acaid||'\');\">
							<img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
						</a>
					</center>' as acao, 
				ac.acadsc
				FROM painel.acao ac 
				$where
				ORDER BY ac.acadsc";
	$cabecalho = array("","Título");
	$db->monta_lista($sql,$cabecalho,100,5,'N','95%',$par2);
//}
?>
<script type="text/javascript">
    function submeter(){
            document.formulario.submit();	
    }

    function removeracao(acaid){
            var conf = confirm("Você realmente deseja excluir esta ação estratégica?");	
            if(conf) {
                    window.location.href = window.location.href + "&action=delete&id="+acaid;	
            }
    }
    function alteraracao(acaid){
        var janela = window.location.href='/painel/painel.php?modulo=sistema/tabelaapoio/cadastroacoes/formulario_acao&acao=A&id='+acaid;
    }
</script>