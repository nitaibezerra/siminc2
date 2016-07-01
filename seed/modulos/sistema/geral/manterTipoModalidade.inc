<?php
if( $_POST['alteraAjax'] ){
    die( dadosAjax($_POST['alteraAjax']) );
}

include  APPRAIZ."includes/cabecalho.inc";
echo '<br>';

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'SEED', 'Manter Tipo de Modalidade de Licitação' );

function dadosAjax($id){
    global $db;
    $sql = "SELECT 
    			tildsc,
    			tilstatus    			
            FROM seed.tipolicitacao
            
            WHERE tilid = $id";
    $dados = $db->carregar( $sql );
    if( $dados ){
        $res =         $dados[0]['tildsc'].'_'.$dados[0]['tilstatus'].'_'.$dados[0]['tilid'];
        return $res;
    }
}

//////////////////////////////ALTERAR//////////////////////////////

if( $_POST['tilid'] == '' ){
    if( $_POST['alterar'] != ''){

$tildsc	= $_REQUEST['tildsc'];

$sql = "UPDATE
			seed.tipolicitacao SET
			
				    tildsc	= '".$tildsc."'
				    
			WHERE
				tilid = ".$_POST["alterar"]."
	";
		$db->executar($sql);
		$db->commit();

		echo("<script>alert('Alterado com Sucesso.')\n</script>");				
		echo("<script>window.location.href = 'seed.php?modulo=sistema/geral/manterTipoModalidade&acao=A'</script>");
		exit();	
	}
}
////////////////SALVAR//////////////////////////////



if($_REQUEST["submetido"]) {

$tildsc	= $_REQUEST['tildsc'];

$sql = "INSERT INTO
		seed.tipolicitacao(

	tildsc,
	tilstatus
)
VALUES(
	'".$tildsc."',
	'A'
)";

	$db->executar($sql);
	$db->commit();
				echo("<script>alert('Gravado com Sucesso!')\n</script>");
				echo("<script>window.location.href = 'seed.php?modulo=sistema/geral/manterTipoModalidade&acao=A'</script>");
				exit();


}

//////////////////////////////EXCLUIR//////////////////////////////

$tilidDel = $_REQUEST['tilid'];

if ($tilidDel){

$sql = "UPDATE 
   					seed.tipolicitacao
   		SET
   					tilstatus='I'
   				WHERE
		   			tilid =	'".$tilidDel."'";
		$db->executar($sql);
		$db->commit();
		
		echo("<script>alert('Excluído com Sucesso.')\n</script>");				
		echo("<script>window.location.href = 'seed.php?modulo=sistema/geral/manterTipoModalidade&acao=A'</script>");
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
                <td class ="SubTituloDireita" align="right">Modalidade: </td>
                <td>
                     <?= campo_texto('tildsc', 'S', $somenteLeitura, '', 50, 600, '', '', 'left', '',  0, 'id="tildsc" onblur="MouseBlur(this);"' ); ?> 
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
                        onclick=\"javascript: alterar('||tilid ||' );\"
                        title=\"Alterar Tipo\"
                     > &nbsp;
                     <img
                        align=\"absmiddle\"
                        src=\"/imagens/excluir.gif\"
                        style=\"cursor: pointer\"
                        onclick=\"javascript: excluir('||tilid ||' );\"
                        title=\"Excluir Tipo\"
                     > ' as acao, 
                tildsc
            FROM
                seed.tipolicitacao
            WHERE
            	tilstatus='A'
                
            ORDER BY tildsc ASC
                ";
    $cabecalho = array("&nbsp;&nbsp;&nbsp;&nbsp;Ação", "Modalidade" );
    $db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '');
    
?>
<script>


function incluirTipo(){
    var tilid			= document.getElementById('tilid');
    var tildsc			= document.getElementById('tildsc');

    if( tildsc.value == ''){
        alert( 'Campo Modalidade é obrigatório.' );
        return false;
    }
    document.formulario.submit();
}
function excluir( tilid ){
    if( confirm( 'Deseja realmente excluir?') ){
        window.location.href = 'seed.php?modulo=sistema/geral/manterTipoModalidade&acao=A&tilid='+tilid;
    }
}
function alterar(id){
 
    var tilid				= document.getElementById('tilid');
    var tildsc				= document.getElementById('tildsc');
    var alterar             = document.getElementById('alterar');
    var req = new Ajax.Request('seed.php?modulo=sistema/geral/manterTipoModalidade&acao=A', {
                                    method:     'post',
                                    parameters: '&alteraAjax='+id,
                                    onComplete: function (res)
                                    {
                                         var arRes       = res.responseText.split("_");
                                                                                  
                                         var Tipo		= arRes[0];
                                         
                                         tildsc.value				= Tipo;
                                         alterar.value				= id;

                                    }
                              });
}
</script>