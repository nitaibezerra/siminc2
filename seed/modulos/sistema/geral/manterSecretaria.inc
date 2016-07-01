<?php
if( $_POST['alteraAjax'] ){
    die( dadosAjax($_POST['alteraAjax']) );
}

include  APPRAIZ."includes/cabecalho.inc";
echo '<br>';

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'SEED', 'Manter Secretaria' );

function dadosAjax($id){
    global $db;
    $sql = "SELECT 
    			secdsc,
    			secstatus    			
            FROM seed.secretaria
            
            WHERE secid = $id";
    $dados = $db->carregar( $sql );
    if( $dados ){
        $res =         $dados[0]['secdsc'].'_'.$dados[0]['secstatus'].'_'.$dados[0]['secid'];
        return $res;
    }
}

//////////////////////////////ALTERAR//////////////////////////////

if( $_POST['secid'] == '' ){
    if( $_POST['alterar'] != ''){

$secdsc	= $_REQUEST['secdsc'];

$sql = "UPDATE
			seed.secretaria SET
			
				    secdsc	= '".$secdsc."'
				    
			WHERE
				secid = ".$_POST["alterar"]."
	";
		$db->executar($sql);
		$db->commit();

		echo("<script>alert('Alterado com Sucesso.')\n</script>");				
		echo("<script>window.location.href = 'seed.php?modulo=sistema/geral/manterSecretaria&acao=A'</script>");
		exit();	
	}
}
////////////////SALVAR//////////////////////////////



if($_REQUEST["submetido"]) {

$secdsc	= $_REQUEST['secdsc'];

$sql = "INSERT INTO
		seed.secretaria(

	secdsc,
	secstatus
)
VALUES(
	'".$secdsc."',
	'A'
)";

	$db->executar($sql);
	$db->commit();
				echo("<script>alert('Gravado com Sucesso!')\n</script>");
				echo("<script>window.location.href = 'seed.php?modulo=sistema/geral/manterSecretaria&acao=A'</script>");
				exit();


}

//////////////////////////////EXCLUIR//////////////////////////////

$secidDel = $_REQUEST['secid'];

if ($secidDel){

$sql = "UPDATE 
   					seed.secretaria
   		SET
   					secstatus='I'
   				WHERE
		   			secid =	'".$secidDel."'";
		$db->executar($sql);
		$db->commit();
		
		echo("<script>alert('Excluído com Sucesso.')\n</script>");				
		echo("<script>window.location.href = 'seed.php?modulo=sistema/geral/manterSecretaria&acao=A'</script>");
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
                <td class ="SubTituloDireita" align="right">Secretaria: </td>
                <td>
                     <?= campo_texto('secdsc', 'S', $somenteLeitura, '', 50, 600, '', '', 'left', '',  0, 'id="secdsc" onblur="MouseBlur(this);"' ); ?> 
                </td>
            </tr>

            <tr>
                <td class ="SubTituloDireita" align="right">  </td>
                <td> <input type="button" name="btSalvar" id="btSalvar" onclick="incluirTipo();" value="GRAVAR">     
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
                        onclick=\"javascript: alterar('||secid ||' );\"
                        title=\"Alterar Tipo\"
                     > &nbsp;
                     <img
                        align=\"absmiddle\"
                        src=\"/imagens/excluir.gif\"
                        style=\"cursor: pointer\"
                        onclick=\"javascript: excluir('||secid ||' );\"
                        title=\"Excluir Tipo\"
                     > ' as acao, 
                secdsc
            FROM
                seed.secretaria
            WHERE
            	secstatus='A'
                
            ORDER BY secdsc ASC
                ";
    $cabecalho = array("&nbsp;&nbsp;&nbsp;&nbsp;Ação", "Secretaria" );
    $db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '');
    
?>
<script>


function incluirTipo(){
    var secid			= document.getElementById('secid');
    var secdsc			= document.getElementById('secdsc');

    if( secdsc.value == ''){
        alert( 'Campo Secretaria é obrigatório.' );
        return false;
    }
    document.formulario.submit();
}
function excluir( secid ){
    if( confirm( 'Deseja realmente excluir?') ){
        window.location.href = 'seed.php?modulo=sistema/geral/manterSecretaria&acao=A&secid='+secid;
    }
}
function alterar(id){
 
    var secid				= document.getElementById('secid');
    var secdsc				= document.getElementById('secdsc');
    var alterar             = document.getElementById('alterar');
    var req = new Ajax.Request('seed.php?modulo=sistema/geral/manterSecretaria&acao=A', {
                                    method:     'post',
                                    parameters: '&alteraAjax='+id,
                                    onComplete: function (res)
                                    {
                                         var arRes       = res.responseText.split("_");
                                                                                  
                                         var Tipo		= arRes[0];
                                         
                                         secdsc.value				= Tipo;
                                         alterar.value				= id;

                                    }
                              });
}
</script>