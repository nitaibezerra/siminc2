<?php
if($_REQUEST['requisicaoAjax'])
{
	$_REQUEST['requisicaoAjax']();
	die;
}

include_once APPRAIZ . "includes/classes/dateTime.inc";
 
//verifica sessão da pagina $_SESSION['ctrid']
verificaSessaoPagina();


function salvar(){
	global $db;
	
	$formataDatax = formata_data_sql( $_REQUEST['ctdataportfiscal'] );
	
	$sql = "UPDATE contratos.ctcontrato 
			SET
				ctnumportfiscal		= ".$_POST['ctnumportfiscal'].",
				ctdataportfiscal	= '".$formataDatax."',
				entidfiscaltit		= null,
				entidfiscalsub		= null
			WHERE
				ctrid = ".$_SESSION['ctrid']."
				RETURNING ctrid
	"; 

	$ctrid = $db->pegaUm($sql);
	$db->commit();
	$_REQUEST['acao'] = "A";
	$db->sucesso('principal/cadFiscalContrato', "");
	die;
} 

function salvar2(){
	global $db;

	if($_FILES["Arquivo"]["name"]){

		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        $dtportaria = date('Y-m-d', strtotime(str_replace('/','-',$_POST['ctdataportfiscal'])));

		$campos	= array("ctrid"				=> $_SESSION['ctrid'],
				"ancdatainclusao" 	=> "now()" ,
				"usucpf"    		=> $_SESSION['usucpf'],
				"tpaid"     		=> $_POST['tpaid'],
				"ancstatus" 		=> "'A'",
                "numportaria"       => $_POST['ctnumportfiscal'],
                "dataportaria"      => "'$dtportaria'"
		);

		$file = new FilesSimec("ctanexo", $campos ,"contratos");

		$arquivoSalvo = $file->setUpload( $_POST['arqdescricao']);

	}
		
	$db->commit();
	$_REQUEST['acao'] = "A";
	$db->sucesso('principal/cadFiscalContrato', "");
	die;
}


if($_POST['action']==1){
	salvar();
}
if($_POST['action']==2){
	salvar2();
}


			
$sql = "SELECT
				c.ctnumportfiscal as ctnumportfiscal,
				c.ctdataportfiscal as ctdataportfiscal,
				c.entidfiscaltit as entidfiscaltit,
				c.entidfiscalsub as entidfiscalsub		
  		FROM contratos.ctcontrato c
  				WHERE
			c.ctrid = ".$_SESSION['ctrid'];



$rsDados = $db->carregar( $sql );
//$rsDados = $db->pegaLinha( $sql );

if($rsDados) extract($rsDados);

//$ctcpffiscaltit = $rsDados[0]['ctcpffiscaltit'];
//$ctcpffiscalsubst = $rsDados[0]["ctcpffiscalsubst"];
//$ctnumportfiscal = $rsDados[0]["ctnumportfiscal"];
//$ctdataportfiscal = $rsDados[0]["ctdataportfiscal"];   




//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, obrigatorio() . ' Indica campo obrigatório' );
montaCabecalhoContrato($_SESSION['ctrid']);

// Por padrão os campos vem habilitados
// $desabilitado   = false;
// $somenteLeitura = 'S';
$acesso 		= verificaPermissaoTelaUsuario();
$desabilitado   = $acesso['desabilitado'];
$somenteLeitura = $acesso['leitura'];

$perfis 			= arrayPerfil();
// $arPerfisBloqEdicao = array(PERFIL_GESTOR_FINANCEIRO_UNIDADE,
// 							PERFIL_CONSULTA_UNIDADE,
// 							PERFIL_CONSULTA_GERAL,
// 							PERFIL_FISCAL_CONTRATO);

// $arDif = array_diff($perfis, $arPerfisBloqEdicao);
// // Bloqueia os campos caso o usuáro só tenha os perfis restritos
// if( count($arDif) == 0 ) {
// 	$desabilitado 	= true;
// 	$somenteLeitura = 'N';
// }

?> 
<style>.link{cursor:pointer}</style>
<script	type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>

    <form name="formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario" enctype="multipart/form-data">
    
    <input type="hidden" name="action" value="" id="action">
    
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

            <tr>
                <td  width="25%" class ="SubTituloDireita" align="right" valign="top">Arquivo Portaria: </td>
                <td>
                <? if(!$desabilitado) : ?>
					<table width="100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
					    <tr>
					        <td>
					            <input type="file" name="Arquivo" id="Arquivo"/>
					            <input type="hidden" name="tpaid" id="tpaid" value="<?=ID_PORTARIA_FISCAL ?>">
					            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
					        </td>      
					    </tr>
					    <tr>
					        <td>
					        	<?= campo_textarea( 'arqdescricao', 'S', 'S', '', 60, 2, 250 ); ?>
					        </td>
					    </tr>
					</table>
				<? endif; ?>

        <tr>
            <td class ="SubTituloDireita" align="right" >Número Portaria:</td>
            <td>
                <?php
                $ctnumportfiscal = $rsDados[0]["ctnumportfiscal"];
                ?>
                <?= campo_texto('ctnumportfiscal', 'S', $somenteLeitura, 'Número da Portaria', 6, 10, '##########', '', 'left', '',  0, 'id="ctnumportfiscal" onblur="MouseBlur(this);"' ); ?>
            </td>
        </tr>

        <tr>
            <td class ="SubTituloDireita" align="right" valign="top">Data Portaria:</td>
            <td>
                <?php
                $ctdataportfiscal = $rsDados[0]["ctdataportfiscal"];
                ?>
                <?= campo_data2( 'ctdataportfiscal','S', (($desabilitado)?'N':'S'), 'Data da Portaria', 'S' ); ?>

                <br><br>
                <? if(!$desabilitado) : ?>
                    <input type="button" name="btnGravar2" value="Gravar" id="btnGravar2" onclick="enviaForm2();">
                    <br><br>
                <? endif; ?>
            </td>
        </tr>

        <tr>
            <td  class ="SubTituloEsquerda" align="left">Lista de Anexos</td>
        </tr>
        <?php
        $sql = "SELECT
                        aqb.ancid, aqb.numportaria, aqb.dataportaria, '<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'?modulo=principal/cadContratoAnexo&acao=A&download=S&arqid=' || arq.arqid || '\';\" /><img src=../imagens/anexo.gif align=absmiddle> ' || arq.arqnome || '.'|| arq.arqextensao ||'</a>', arq.arqdescricao
                     FROM
                        ((public.arquivo arq INNER JOIN contratos.ctanexo aqb
                        ON arq.arqid = aqb.arqid) INNER JOIN contratos.tipoanexo tarq
                        ON tarq.tpaid = aqb.tpaid) INNER JOIN seguranca.usuario usu
                        ON usu.usucpf = arq.usucpf
                    WHERE
                        arq.arqstatus = 'A' AND  aqb.ctrid = " .$_SESSION['ctrid']."
                    AND tarq.tpaid = ".ID_PORTARIA_FISCAL."";


        $cabecalho = array("Ações", "Nº da portaria", "Data da portaria", "Arquivo", "Descrição");

        $rs = $db->carregar($sql);

        if ($rs) {
            for ($x = 0; $x < count($rs); $x++) {
                $server = $_SERVER['SERVER_NAME'];
                $endereco = $_SERVER ['REQUEST_URI'];
                array_unshift($rs[$x], "<a href='$server$endereco&excluir={$rs[$x]['ancid']}' style='cursor:pointer'><img src='../imagens/excluir.gif''</a>");
                unset($rs[$x]['ancid']);
            }
        }

        $db->monta_lista_array($rs,$cabecalho,1000,1000,"N","100%");

        ?>
                </td> 
            </tr>
            <tr>
            	<td class="SubTituloDireita" colspan="2">&nbsp;</td>
            </tr>
        <tr>
            <td class="SubTituloDireita" align="right">Gestor:</td>
            <td align="left"><input type="button" name="pesquisar_gestor" value="Adicionar" style="cursor: pointer;" onclick="inserirFiscalSub();" <?php if($somenteLeitura=="N") echo "disabled"; ?>>
            </td>
        </tr>
        <tr>
            <td colspan="2" align="center" id="td_lista_gestor" >
                <?php listarGestorContrato()?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" colspan="2">&nbsp;</td>
        </tr>
            <tr>
            	<td class="SubTituloDireita" align="right">Fiscal:</td>
            	<td align="left"><input type="button" name="pesquisar_fiscal" value="Adicionar" style="cursor: pointer;" onclick="inserirFiscal();" <?php if($somenteLeitura=="N") echo "disabled"; ?>>
            	</td>           	
            </tr>
            <tr>
            	<td colspan="2" align="center" id="td_lista_fiscal">
            		<?php listarFiscalContrato(); ?>
            	</td>
            </tr>
            <tr>
            	<td class="SubTituloDireita" colspan="2">&nbsp;</td>
            </tr>



      </table>
      </form>     

<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript">
jQuery.noConflict();

var d = document;

function enviaForm(){
 	var nomeform 		= 'formulario';
	var submeterForm 	= true;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();
	
	campos[0]			= "ctnumportfiscal";		
	campos[1]			= "ctdataportfiscal";
		
					 
	tiposDeCampos[0] 	= "numero";
	tiposDeCampos[1] 	= "data";

/*	if(d.formulario.ctcpffiscaltit.value == ''){
		alert("Selecione o Fiscal Titular");
		return false;
	}

	if(d.formulario.ctcpffiscalsubst.value == ''){
		alert("Selecione o Fiscal Substituto");
		return false;
	}
*/


	d.formulario.action.value='1';
	
	validaForm(nomeform, campos, tiposDeCampos, submeterForm );
}

function enviaForm2(){
	var msg = new Array();
	
	if(d.formulario.Arquivo.value == ''){
		msg.push("Selecione um arquivo");
	}

	if(d.formulario.arqdescricao.value == ''){
		msg.push("Informe a descrição do arquivo");
	}
	
	if ( msg.length > 0 ){
		alert( msg.join("\n") );
		return false;
	}else{
		d.formulario.action.value='2';
		d.formulario.submit();
	}
}

var caminho_atual = "contratos.php";
function inserirFiscal(entidfiscaltit){
	if (entidfiscaltit){ 
			return windowOpen( caminho_atual + '?modulo=principal/inserir_fiscal&acao=A&busca=entnumcpfcnpj&campo=1&entid=' + entidfiscaltit,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}else{
			return windowOpen( caminho_atual + '?modulo=principal/inserir_fiscal&acao=A&campo=1','blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}
}

function inserirFiscalSub(entidfiscalsub){	
	if (entidfiscalsub){ 
		return windowOpen( caminho_atual + '?modulo=principal/inserir_fiscal&acao=A&busca=entnumcpfcnpj&campo=2&entid=' + entidfiscalsub,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
	}else{
		return windowOpen( caminho_atual + '?modulo=principal/inserir_fiscal&acao=A&campo=2','blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
	}
}


function editarFiscal(entid)
{
	inserirFiscal(entid);
}

function editarGestor(entid)
{
	inserirFiscalSub(entid);
}

function removerFiscal(fstid)
{
	if(confirm("Deseja realmente remover o Fiscal?")){
		jQuery.ajax({
		   type: "POST",
		   url: window.location,
		   data: "requisicaoAjax=removerFiscal&fscid="+fstid,
		   success: function(msg){
               alert("removido com sucesso");
               location.reload();
			   //$("#td_lista_fiscal").html(msg);
		   }
		 });
	}
}

function removerGestor(gstid)
{
	if(confirm("Deseja realmente remover o Gestor?")){
		$.ajax({
		   type: "POST",
		   url: window.location,
		   data: "requisicaoAjax=removerGestor&gscid="+gstid,
		   success: function(msg){
			   $("#td_lista_gestor").html(msg);
		   }
		 });
	}
}

</script>