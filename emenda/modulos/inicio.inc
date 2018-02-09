<?
 /*
   Sistema Simec
   Setor responsvel: MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Cristiano Cabral (cristiano.cabral@gmail.com)
   Mdulo:inicio.inc
   Finalidade: permitir abrir a página inicial
    */

//Chamada de programa
//include  APPRAIZ."includes/cabecalho.inc";

// altera o ano de exercício (caso o usuário solicite)
if ( $_REQUEST['exercicio'] AND $_SESSION['exercicio'] != $_REQUEST['exercicio'] ) {
	$_SESSION['exercicio'] = $_REQUEST['exercicio'];  
	$exercicio = $_SESSION['exercicio'];
	$sql = sprintf(
		"SELECT prsexercicioaberto FROM %s.programacaoexercicio WHERE prsano = '%s'",
		$_SESSION['sisdiretorio'],
		$_REQUEST['exercicio']
	);
	$_SESSION['exercicioaberto'] = $db->pegaUm( $sql );
}

$usucpf = $_SESSION['usucpf'];
$pflcod = pegaPerfil($usucpf);

unset($_SESSION["emenda"]);

/*if( $_SESSION['exercicio'] >= '2012' ){
	header( "Location: ../muda_sistema.php?sisid=128" );
} else {*/
	if($db->testa_superuser()){
		include  APPRAIZ."includes/cabecalho.inc";
		//header( "Location: ?modulo=principal/listaPlanoTrabalho&acao=A" );
		header( "Location: ?modulo=principal/listaDeEmendas&acao=A" );
		?>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2">
			<tr bgcolor="#e7e7e7">
			  <td><h1>Bem-vindo</h1></td>
			</tr>
		</table>
		<div class="pagina">
	    <table align="center" width="95%" border="0" cellpadding="0" cellspacing="0">
	    <tr>
	        <td class="corpo" valign="middle">
	        <table align="center" width="100%" border="0" cellpadding="0" cellspacing="0">
	            <tr>
	              <td align="center">
	                <div class="linha">          
	                    <div class="tile" style="background-color: #DAA520">Tile 1</div>
	                  	<div class="tile" style="background-color: #4682B4">Tile 2</div>
	                    <div class="tile" style="background-color: #CD0000">Tile 3</div>
	                    <div class="tile" style="background-color: #2E8B57">Tile 4</div>
	                </div></td>
	            </tr>
	            <tr>
	              <td align="center">
	                <div class="linha">          
	                    <div class="tile" style="background-color: #D1512C">Tile 1</div>
	                    <div class="tile" style="background-color: #AE193E">Tile 2</div>
	                    <div class="tile" style="background-color: #5B39B6">Tile 3</div>
	                    <div class="tile" style="background-color: #9BCEBA">Tile 4</div>
	                </div></td>
	            </tr>
	            <tr>
	              <td align="center">
	                <div class="linha">          
	                    <div class="tile" style="background-color: #FBA919">Tile 1</div>
	                    <div class="tile" style="background-color: #2D87EF">Tile 2</div>
	                    <div class="tile" style="background-color: #009900">Tile 3</div>
	                    <div class="tile" style="background-color: #474747">Tile 4</div>
	                </div></td>
	            </tr>
	        </table>
	        </td>
	    </tr>
	    </table>
	    </div>
		<?
	}else{
		if($pflcod == SUPER_USUARIO || $pflcod == ADMINISTRADOR_INST){
			//header( "Location: ?modulo=principal/listaPlanoTrabalho&acao=A" );
			header( "Location: ?modulo=principal/listaDeEmendas&acao=A" );
		} elseif ($pflcod == INSTITUICAO_BENEFICIADA){
			header( "Location: ?modulo=principal/listaInstituicoesEntBen&acao=A&retorno=listaPlanoTrabalho" );
		} elseif( $pflcod == ADMINISTRADOR_REFORMULACAO ){
			header( "Location: ?modulo=principal/listaPlanoTrabalho&acao=A" );
		} else {
			header( "Location: ?modulo=principal/listaDeEmendas&acao=A" );
		}
	}
//}

?>
<script type="text/javascript">
	$(function(){
		$(".tile").mousedown(function(){       
			$(this).addClass("selecionado");
		});
	 
		$(".tile").mouseup(function(){     
			$(this).removeClass("selecionado");
		});
	});
</script>
<style type="text/css">

/*@font-face { font-family: Century; src: url('GOTHIC.ttf'); }*/

.corpo{
	background: rgb(51,51,51); 
	height: 500px;
}

.pagina{
    width:auto;
    height:auto;
}
 
.linha{
    width:auto;
    padding:5px;
    height:auto;
    display:table;
}

.tile{
    height:120px;   
    width:210px;  
    float:left;
    margin:0 5px 0 0;
    padding:2px;
	border: 3px rgb(51,51,51) solid;
	cursor: pointer;
	line-height:120px;
	-webkit-border-radius: 15px;
	-moz-border-radius: 15px;
	border-radius: 15px;	
	text-align: center;	
	color: #fff; 
	font-weight: bold; 
	font-family: Century; 
	font-size:14px;
}

.tile:hover{
	border: 3px #696969 solid;
	opacity:0.8;
	filter:alpha(opacity=80);
}

.selecionado{
    background-color: rgb(51,51,51);
    border: 3px  solid !important;
}
</style>

