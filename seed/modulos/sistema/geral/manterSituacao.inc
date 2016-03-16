<?php
if( $_POST['alteraAjax'] ){
    die( dadosAjax($_POST['alteraAjax']) );
}

include  APPRAIZ."includes/cabecalho.inc";
echo '<br>';

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'SEED', 'Manter Situação' );

function dadosAjax($id){
    global $db;
    $sql = "SELECT 
    			sitdsc,
    			sitstatus    			
            FROM seed.situacao
            
            WHERE sitid = $id";
    $dados = $db->carregar( $sql );
    if( $dados ){
        $res =         $dados[0]['sitdsc'].'_'.$dados[0]['sitstatus'].'_'.$dados[0]['sitid'];
        return $res;
    }
}

//////////////////////////////ALTERAR//////////////////////////////

if( $_POST['sitid'] == '' ){
    if( $_POST['alterar'] != ''){

$sitdsc	= $_REQUEST['sitdsc'];

$sql = "UPDATE
			seed.situacao SET
			
				    sitdsc	= '".$sitdsc."'
				    
			WHERE
				sitid = ".$_POST["alterar"]."
	";
		$db->executar($sql);
		$db->commit();

		echo("<script>alert('Alterado com Sucesso.')\n</script>");				
		echo("<script>window.location.href = 'seed.php?modulo=sistema/geral/manterSituacao&acao=A'</script>");
		exit();	
	}
}
////////////////SALVAR//////////////////////////////



if($_REQUEST["submetido"]) {

$sitdsc	= $_REQUEST['sitdsc'];

$sql = "INSERT INTO
		seed.situacao(

	sitdsc,
	sitstatus
)
VALUES(
	'".$sitdsc."',
	'A'
)";

	$db->executar($sql);
	$db->commit();
				echo("<script>alert('Gravado com Sucesso!')\n</script>");
				echo("<script>window.location.href = 'seed.php?modulo=sistema/geral/manterSituacao&acao=A'</script>");
				exit();


}

//////////////////////////////EXCLUIR//////////////////////////////

$sitidDel = $_REQUEST['sitid'];

if ($sitidDel){

		$sql = "UPDATE
			seed.situacao SET
			
				    sitstatus	= 'I'
				    
			WHERE
				sitid =	'".$sitidDel."'";

		$db->executar($sql);
		$db->commit();	
		
		echo("<script>alert('Excluído com Sucesso.')\n</script>");				
		echo("<script>window.location.href = 'seed.php?modulo=sistema/geral/manterSituacao&acao=A'</script>");
		exit();	
	}

?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

 <form name = "formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <input type="hidden" name="submetido" value="1">
    <input type ="hidden" name="alterar" id="alterar" value="">
           
            <tr>
                <td class ="SubTituloDireita" align="right">Situação: </td>
                <td>
                     <?= campo_texto('sitdsc', 'S', $somenteLeitura, '', 50, 50, '', '', 'left', '',  0, 'id="sitdsc" onblur="MouseBlur(this);"' ); ?> 
                </td>
            </tr>

            <tr>
                <td class ="SubTituloDireita" align="right">  </td>
                <td> <input type="button" name="btSalvar" id="btSalvar" onclick="incluirSituacao();" value="GRAVAR">     
                </td>
            </tr>
     </table>
     </form>
          
    <?php
    $sql = "SELECT
                '<img
                        align=\"absmiddle\"
                        src=\"/imagens/alterar.gif\"
                        style=\"cursor: pointer\"
                        onclick=\"javascript: alterar('||sitid ||' );\"
                        title=\"Alterar Situação\"
                     > &nbsp;
                     <img
                        align=\"absmiddle\"
                        src=\"/imagens/excluir.gif\"
                        style=\"cursor: pointer\"
                        onclick=\"javascript: excluir('||sitid ||' );\"
                        title=\"Excluir Situação\"
                     > ' as acao, 
                sitdsc
            FROM
                seed.situacao
                
            WHERE
            	sitstatus	= 'A'
                
            ORDER BY sitdsc ASC
                ";
    $cabecalho = array("&nbsp;&nbsp;&nbsp;&nbsp;Ação", "Situação" );
    $db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '');
  
?>
<script>


function incluirSituacao(){
    var sitid			= document.getElementById('sitid');
    var sitdsc			= document.getElementById('sitdsc');

    if( sitdsc.value == ''){
        alert( 'Campo Situação é obrigatório.' );
        return false;
    }
    document.formulario.submit();
}
function excluir( sitid ){
    if( confirm( 'Deseja realmente excluir?') ){
        window.location.href = 'seed.php?modulo=sistema/geral/manterSituacao&acao=A&sitid='+sitid;
    }
}
function alterar(id){
 
    var sitid				= document.getElementById('sitid');
    var sitdsc				= document.getElementById('sitdsc');
    var alterar             = document.getElementById('alterar');
    var req = new Ajax.Request('seed.php?modulo=sistema/geral/manterSituacao&acao=A', {
                                    method:     'post',
                                    parameters: '&alteraAjax='+id,
                                    onComplete: function (res)
                                    {
                                         var arRes       = res.responseText.split("_");
                                                                                  
                                         var situacao		= arRes[0];
                                         
                                         sitdsc.value				= situacao;
                                         alterar.value				= id;

                                    }
                              });
}
</script>