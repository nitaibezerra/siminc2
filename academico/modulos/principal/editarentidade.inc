<?php
$_SESSION['academico']['entid'] = $_REQUEST['entidunidade'] ? $_REQUEST['entidunidade'] : $_SESSION['academico']['entid'];
if($_REQUEST['exec_function']) {
	header('Content-Type: text/html; charset=ISO-8859-1');
	$_REQUEST['orgid'] = $_SESSION['academico']['orgid'];
	$_REQUEST['entid'] = $_SESSION['sig_var']['entid'];
	$_REQUEST['exec_function']($_REQUEST);
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php'; 
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";
echo "<br/>";

if($_REQUEST['entidunidade'] && $_REQUEST['tpeid'] && $_REQUEST['iscampus'] || (!$_SESSION['sig_var']['entid'] || !$_SESSION['academico']['orgid'])) {
	$_SESSION['sig_var']['entid'] = $_REQUEST['entidunidade'] ? $_REQUEST['entidunidade'] : $_SESSION['academico']['entid'];
//	$_SESSION['sig_var']['tpeid'] = $_REQUEST['tpeid'] ? $_REQUEST['tpeid'] : $_SESSION['academico']['orgid'];
	$_SESSION['sig_var']['iscampus'] = $_REQUEST['iscampus'];
}

$perfilNotBloq = array(
    PERFIL_IFESCADBOLSAS,
    PERFIL_IFESCADCURSOS,
    PERFIL_IFESCADASTRO,
    PERFIL_MECCADBOLSAS,
    PERFIL_MECCADCURSOS,
    PERFIL_MECCADASTRO,
    PERFIL_ADMINISTRADOR,
    PERFIL_REITOR,
    PERFIL_INTERLOCUTOR_INSTITUTO,
    PERFIL_PROREITOR);
// Verificando a segurança
$permissoes = verificaPerfilAcademico($perfilNotBloq);
validaAcessoTipoEnsino($permissoes['vertipoensino'],$_SESSION['academico']['orgid']);
// Fim segurança

// Monta as abas
if ($_SESSION['baselogin'] == "simec_desenvolvimento"){
	$abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57276' : '57135'; //Dados Desenv
}else{
	$abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57267' : '57135';
}
#$db->cria_aba($abacod_tela,$url,$parametros);
$autoriazacaoconcursos = new autoriazacaoconcursos();
?>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./geral/js/academico.js"></script>
<script language="JavaScript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script>
function controleagrupamento(cnt) {
	if(cnt.checked) {
		switch(cnt.value) {
			case 'porunidade':
				ajaxatualizar('exec_function=carregarItensEntidadeVisualizar','itensEntidadeVisualizar');
				aplicarmascara();			
				break;
			case 'porcampus':
				ajaxatualizar('exec_function=carregarItensEntidadeVisualizar&porcampus=sim','itensEntidadeVisualizar');
				aplicarmascara();
				break;
		}
	}
}
</script>
<?
monta_titulo( 'Informações Gerenciais', '' );
?>
<div class=''>
    <div class='col-md-2' style='position:static;'>
        <?=cria_aba_2($abacod_tela, $url, $parametros);?>
    </div>
    <div class='col-md-9' style='position:static;'>
        <form action="?modulo=principal/editarcampus&acao=A" method="post" name="formulario" id="formulario">
            <?
            //montando cabeçalho
            echo $autoriazacaoconcursos->cabecalho_entidade($_SESSION['sig_var']['entid']);
            ?>
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
                <tr>
                    <td class="SubTituloDireita">Agrupamento :</td>
                    <td><input type="radio" name="agrup" onclick="controleagrupamento(this);" value="porunidade" checked> Por unidade <input type="radio" name="agrup" value="porcampus" onclick="controleagrupamento(this);"> Por
                    <?
                    // Verificando o tipoensino
                    $tipolocalidade = definirtipolocalidade($_SESSION['academico']['orgid']);
                    echo $tipolocalidade['nome'];

                    ?>
                    </td>
                </tr>
            </table>

            <div id="itensEntidadeVisualizar">
            </div>

        <script>
            ajaxatualizar('exec_function=carregarItensEntidadeVisualizar','itensEntidadeVisualizar');
        </script>

            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
                <tr bgcolor="#C0C0C0"><td colspan="2" align="center">&nbsp;</td></tr>
            </table>
        </form>
        <script type="text/javascript" src="../includes/wz_tooltip.js"></script>
        <script>
            aplicarmascara();
            function aplicarmascara() {
                var form = document.getElementById('formulario');
                for (var j=0; j < form.length; j++){
                    if(form[j].value != "" && form[j].type == "hidden" && form[j].name.substr(0,14) == "gravacaocampo_") {
                        form[j].onkeyup();
                        form[j].parentNode.innerHTML = form[j].parentNode.innerHTML+form[j].value;
                        form[j].parentNode.style.textAlign = 'right';
                    }
                }
            }
        </script>
    </div>
</div>


