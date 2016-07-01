<?php
verificaSessao( 'obra' );
$empid = $_SESSION['obras2']['empid'];
$obrid = $_SESSION['obras2']['obrid'];


switch ( $_REQUEST['requisicao'] ){
	case 'apagar': 
		$regAtividade = new RegistroAtividade( $_REQUEST['rgaid'] );
            
                if($_REQUEST['rgaid'] && $regAtividade->rgaautomatica == 'f'  && possuiPerfil(PFLCOD_SUPER_USUARIO)){
                    $regAtividade->rgastatus = 'I';		
                    $regAtividade->salvar();

                    $db->commit();
                    die("<script>
                            alert('Operação realizada com sucesso!'); 
                            window.location = '?modulo=principal/listaRegistroAtividade&acao={$_GET['acao']}';
                         </script>");
                }else{
                    die("<script>
                            alert('Você não possui permissão para excluir esse Registro!!'); 
                            window.location = '?modulo=principal/listaRegistroAtividade&acao={$_GET['acao']}';
                         </script>");
                }

    case 'tornarimportante':


        $regAtividade = new RegistroAtividade( $_REQUEST['rgaid'] );

        $regAtividade->rgaimp = 't';
        $regAtividade->alterar($dados['rgaimp']);
        $regAtividade->salvar();
        break;

    case 'retirarimportancia':
        $regAtividade = new RegistroAtividade( $_REQUEST['rgaid'] );

        $regAtividade->rgaimp = 'f';
        $regAtividade->alterar($dados['rgaimp']);
        $regAtividade->salvar();
        break;
	case "download":
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$arqid = $_REQUEST['arqid'];
		$file = new FilesSimec();
	    $arquivo = $file->getDownloadArquivo($arqid);
		die();



}

include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";


$db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,$url,$parametros);

echo cabecalhoObra($obrid);

monta_titulo($titulo_modulo, '');

extract( $_POST );

$habilitado = true;
$habilita 	= 'S';
if( possui_perfil( array(PFLCOD_CONSULTA_UNIDADE, PFLCOD_CONSULTA_ESTADUAL, PFLCOD_CONSULTA_TIPO_DE_ENSINO) ) ){
	$habilitado = false;
	$habilita 	= 'N';
}
?> 
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<form name="formulario" id="formulario" method="post" action="">
	<input type="hidden" name="excluirRegistro" id="excluirRegistro" value="excluir" />
		<table id="idRegistroAtividade" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding=3 align="center">
			<tr>
				<td style="font-weight:bold; color:black; font-family:Arial,Verdana; background-color:#f0f0f0; text-align:left; " colspan="2">
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Argumentos de Pesquisa
				</td>
			</tr>
			
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Data da Criação:</td>
				<td>
					<?php 
					echo "Início " . campo_data2('rgadtinclusaoinicio', 'N', 'S', '', 'N');
					echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
					echo "Fim " . campo_data2('rgadtinclusaofim', 'N', 'S', '', 'N' );												
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Usuário:</td>
				<td>
					<?php 
					echo campo_texto( 'usunome', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, '');
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Descrição simplificada:</td>
				<td>
					<?php 
					echo campo_texto( 'rgadscsimplificada', 'N', 'S', '', 47 , 60, '', '', 'left', '', 0, '');	
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Descrição detalhada:</td>
				<td>
					<?php 
					echo campo_texto( 'rgadsccompleta', 'N', 'S', '' , 47 , 60, '', '', 'left', '', 0, '');	
					?>
				</td>
			</tr>
			<tr bgcolor="#D0D0D0">
				<td style="width: 190px;"></td>
				<td>
					<input type="button" name="visualizarRegAtividades" value="Visualizar" onclick="this.disabled=true; document.getElementById('formulario').submit();" style="cursor: pointer;"/>
					<input type="button" name="verTodosRegAtividades" value="Ver Todas" onclick="this.disabled=true; window.location='?modulo=principal/listaRegistroAtividade&acao=O'" style="cursor: pointer;"/>
				</td>
			</tr>
	</table>
</form>
<?php
$param = $_POST;
if ( $_GET['acao'] == 'A' ){
	$param['empid'] = $_SESSION['obras2']['empid'];
}else{
	$param['obrid'] = $_SESSION['obras2']['obrid'];
}
$param['habilitado'] = $habilitado;

$registro  = new RegistroAtividade();
$sql 	   = $registro->listaSql( $param );

$cabecalho = array( "Ação", "Seq.", "Descrição", "Tipo", "Data", "Arquivo", "Inserido por" );
$db->monta_lista($sql,$cabecalho,100,5,'N','center','');

if ( $habilitado ):
?>
<table class="tabela" bgcolor="#FFFFFF" cellspacing="1" cellpadding=3 align="center">
	<tr>
		<td style="font-weight: bold;">
			<a style="cursor: pointer;" onclick="javascript:popupRegAtividades();" title="Clique para inserir um novo Registro de Atividades">
			<img src="../imagens/obras/incluir.png" style="width: 15px; vertical-align: middle;"/>Inserir novo</a>
		</td>
	</tr>
</table>
<?php
endif;
?>
<script type="text/javascript">

    $(document).ready(function (){
        $('img[src="/imagens/estrela.png"]').parent().parent().parent().attr('style','background:#FFED32');
    });




$(document).ready(function (){
	$('.download').click(function(){
		var id = $(this).attr('id');
		DownloadArquivo( id );
	});

    $('.acoes-<?=$_SESSION['usucpf']?>').show();
});

<!--
//Função que abre a popup para inclusão
function popupRegAtividades(){
	open("?modulo=principal/cadRegistroAtividade&acao=<?php echo $_GET['acao'] ?>","inserirRegistro","width=800,height=500,toolbar=no,scrollbars=yes,status=yes");
}

function alterarReg( rgaid ){
	janela = window.open('?modulo=principal/cadRegistroAtividade&acao=<?php echo $_GET['acao'] ?>&rgaid=' + rgaid, 'inserirRegistro', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=800,height=500' ); 
	janela.focus();
}

function excluirReg( rgaid ){
        <?
        $objObras = new Obras($_SESSION['obras2']['obrid']);
        $blockEdicao = $objObras->verificaObraVinculada();
        if($blockEdicao){
            echo 'var blockEdicao = true;';
        }else{
            echo 'var blockEdicao = false;';
        }
        ?>
	if ( confirm('Deseja apagar esta atividade?') ){
            if(blockEdicao){
                alert('Você não pode editar os dados da Obra Vinculada.');
            }else{
		location.href = '?modulo=principal/listaRegistroAtividade&acao=<?php echo $_GET['acao'] ?>&rgaid=' + rgaid + '&requisicao=apagar';
            }
	}
}

    function retirarImportancia( rgaid ){
                     location.href = '?modulo=principal/listaRegistroAtividade&acao=<?php echo $_GET['acao'] ?>&rgaid=' + rgaid + '&requisicao=retirarimportancia';
    }
    function tornarImportante( rgaid ){
        location.href = '?modulo=principal/listaRegistroAtividade&acao=<?php echo $_GET['acao'] ?>&rgaid=' + rgaid + '&requisicao=tornarimportante';
    }

    function verReg( rgaid ){
    janela = window.open('?modulo=principal/verRegistroAtividade&acao=A&rgaid=' + rgaid, 'inserirRegistro', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=800,height=500' );
    janela.focus();
}


DownloadArquivo = function(arqid){
	window.location = '?modulo=principal/listaRegistroAtividade&acao=<?php echo $_GET['acao'] ?>&requisicao=download&arqid='+arqid;
}

//-->
</script>