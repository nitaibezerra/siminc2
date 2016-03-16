<?php
/**
 * Sistema Integrado de Monitoramento do Ministério da Educação
 * Setor responsvel: SPO/MEC
 * Desenvolvedor: Desenvolvedores Simec
 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>, Alexandre Soares Diniz
 * Programadores: Renê de Lima Barbosa <renedelima@gmail.com>, Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
 * Módulo: segurança
 * Finalidade: Consultar informações a resppeito das contas dos usuários
 * Data de criação:
 * Última modificação: 29/08/2006
 */

$modulo = $_REQUEST['modulo'] ;
$encontrou = false;


if ($_POST['ajaxsisid']){
    monta_perfil($_POST['ajaxsisid']);
    exit;
}

$habil = 'S';

if ( $_REQUEST['formulario'] ) {
    $usucpf  = $_REQUEST['usucpf'];
    $usunome = $_REQUEST['usunome'];
    $codigo  = $_REQUEST['codigo'];
    $suscod  = $_REQUEST['suscod'];
    $pflcod  = $_REQUEST['pflcod'];
    $muncod  = $_REQUEST['muncod'];
    $usuchaveativacao = $_REQUEST['usuchaveativacao'];
    $sitperfil = $_REQUEST['sitperfil'];
} else {
    $codigo = $_SESSION['sisid'];
}

if(isset($_REQUEST['servico']) &&  $_REQUEST['servico']== 'listar_mun'){
    $sql = "SELECT muncod, mundescricao as mundsc
		FROM territorios.municipio 
		WHERE estuf = '".$_REQUEST['estuf']."' ORDER BY mundsc";
    $dados = $db->carregar($sql);

    $enviar = '';
    if($dados) $dados = $dados; else $dados = array();
    $enviar .= "<option value=\"\"> Todos </option> \n";
    foreach ($dados as $data) {
        $enviar .= "<option value= ".$data['muncod'].">  ".simec_htmlentities($data['mundsc'])." </option> \n";
    }

    die($enviar);
}

?>
<?php include APPRAIZ . "includes/cabecalho.inc"; ?>
<br/>
<?php

$db->cria_aba( $abacod_tela, $url, $parametros );
monta_titulo( $titulo_modulo, '' );

?>
<form method="POST" name="formulario">
<script type="text/javascript" src="../includes/prototype.js"></script>
<input type="hidden" name="formulario" value="1"/>
<input type=hidden name="modulo" value="<?= $modulo ?>"/>
<input type=hidden name="act" value="0"/>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
    <td align='right' class="SubTituloDireita">CPF (ou parte do CPF):</td>
    <td><?=campo_texto('usucpf','','','',16,14,'###.###.###-##','', '', '', '', '', '', '', "this.value=mascaraglobal('###.###.###-##',this.value);");?></td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita">Nome completo (ou parte do nome):</td>
    <td><?=campo_texto('usunome','','','',50,50,'','');?></td>
</tr>
<!--
		<tr>
		<td align='right' class="SubTituloDireita">Município:</td>
			<td><?
				//campo_texto('muncod','','','',10,7,'#######','');
			?></td>
		</tr>
		-->
<tr bgcolor="#F2F2F2">
    <td align='right' class="SubTituloDireita">Unidade Federativa:</td>
    <td>
        <?php
        $sql = "SELECT regcod AS codigo, regcod||' - '||descricaouf AS descricao FROM uf WHERE codigoibgeuf IS NOT NULL ORDER BY 2";
        $db->monta_combo("regcod",$sql,"","&nbsp;",'listar_municipios','');
        //echo  obrigatorio();
        ?>
    </td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita">Município:</td>
    <td>
        <div id="muncod_on" style="display:none;">
            <select
                id="muncod"
                name="muncod"
                onchange=""
                class="CampoEstilo"
                <?
                ///3campo  do monta combo - verifica se é editavel
                //$editavel == 'S' ? "" : 'disabled="disabled"'
                ?>
                ></select>
            <?
            //echo obrigatorio();
            ?>
        </div>
        <div id="muncod_off" style="color:#909090;">Selecione uma Unidade Federativa.</div>
    </td>
</tr>
<tr bgcolor="#C0C0C0">
    <td ></td>
    <td><input type='button' class="botao" name='consultar' value='Consultar' onclick="ProcuraUsuario('')"></td>
</tr>
</table>
</form>
<?php
unset( $nlinhas );

$where = array();
if ($_REQUEST['usucpf']) {
    $usucpf = str_replace(array('.', ' ', '-'), '', $_REQUEST['usucpf']);
    $where[] = " u.usucpf ilike ('%{$usucpf}%')";
}
if ($_REQUEST['usunome']) {
    $where[] = " u.usunome ilike ('%{$_REQUEST['usunome']}%')";
}
if ($_REQUEST['regcod_disable']) {
    $where[] = " m.estuf = '{$_REQUEST['regcod_disable']}'";
}
if ($_REQUEST['muncod']) {
    $where[] = " u.muncod = '{$_REQUEST['muncod']}'";
}

$sql = " SELECT
            '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Cadastro \" onclick=\"alterar_usu(' || '''' || u.usucpf || '''' || ')\">' as acao,
            u.usucpf, usunome, '(' || usufoneddd || ') ' || usufonenum, usuemail, m.estuf, m.mundescricao
        from seguranca.cpfinvalido cpf
            inner join seguranca.usuario u on u.usucpf = cpf.usucpf
            left join territorios.municipio m on m.muncod = u.muncod";

$sql .= count($where) ? ' where ' . implode(' and ', $where) : ' ';

$sql .= " ORDER BY usunome ";

$cabecalho = array('ID', 'CPF', 'Nome', 'Telefone', 'E-mail', 'UF', 'Município');

$db->monta_lista( $sql, $cabecalho, 10000, 20, '', '' ,'' );


function monta_perfil($sisid = null){
    global $db;

    if (!$sisid){
        return false;
    }

    $sql = "SELECT
			 pflcod AS codigo,
			 pfldsc AS descricao
			FROM
			 seguranca.perfil
			WHERE
			 pflstatus = 'A' AND
			 sisid = $sisid";

    $db->monta_combo( "pflcod", $sql, 'S', 'Todos', '', '' );
}
?>
<script language="javascript"><!--

    function envia_email( cpf ){
        e = "?modulo=sistema/geral/envia_email&acao=A&cpf=" + cpf;
        window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=590,height=490");
    }


    function montar_perfil(sisid){
        if (sisid == ''){
            return;
        }
        if (document.getElementsByName('pflcod')){
            document.getElementsByName('pflcod')[0].disabled = true;
        }
        // Faz uma requisição ajax
        var req = new Ajax.Request('?modulo=sistema/usuario/consusuario&acao=A', {
            method:      'post',
            parameters:  '&ajaxsisid=' + sisid,
            asynchronous: false,
            onComplete:   function (res)
            {

                document.getElementById('pflid').innerHTML = res.responseText;

            }
        });
    }

    function listar_municipios( estuf )
    {
        validar_mun = true;
        var div_on = document.getElementById( 'muncod_on' );
        var div_off = document.getElementById( 'muncod_off' );
        div_on.style.display = 'block';
        div_off.style.display = 'none';

        return new Ajax.Updater('muncod', '<?=$_SESSION['sisdiretorio'] ?>.php?modulo=sistema/usuario/consusuario&acao=<?=$_REQUEST['acao'] ?>',
            {

                method: 'post',
                parameters: '&servico=listar_mun&estuf=' + estuf,
                onComplete: function(res)
                {
                    // alert(res.responseText);
                }
            });
    }

    function listar_municipios2( regcod )
    {


        var campo_select = document.getElementById( 'muncod' );
        while( campo_select.options.length )
        {
            campo_select.options[0] = null;
        }
        campo_select.options[0] = new Option( '', '', false, true );

        var div_on = document.getElementById( 'muncod_on' );
        var div_off = document.getElementById( 'muncod_off' );

        if ( !lista_mun[regcod] )
        {
            validar_mun = false;
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            return;
        }

        validar_mun = true;
        div_on.style.display = 'block';
        div_off.style.display = 'none';
        var j = lista_mun[regcod].length;
        for ( var i = 0; i < j; i++ )
        {
            campo_select.options[campo_select.options.length] = new Option( lista_mun[regcod][i][1], lista_mun[regcod][i][0], false, lista_mun[regcod][i][0] == '<?= $muncod ?>' );
        }
        for ( i=0; i< campo_select.options.length; i++ )
        {
            if ( campo_select.options[i].value == '<?= $muncod ?>' ) {
                campo_select.options[i].selected = true;
            }
        }
    }




    function ProcuraUsuario(tipo) {
        if ( document.formulario.usunome ) {
            if(tipo == 'xls'){
                document.formulario.act.value = "visualizaXls";
            } else {
                document.formulario.act.value = "procura";
            }
            document.formulario.submit()
        }
    }

    function consultar_usu( usucpf ){
        <?php if ( $_SESSION['sisid'] == 147 ): ?>
        window.location = "<?=$_SESSION['sisarquivo']?>.php?modulo=sistema2/usuario2/cadusuario&acao=C&usucpf="+ usucpf;
        <?php else: ?>
        window.location = "<?=$_SESSION['sisarquivo']?>.php?modulo=sistema/usuario/cadusuario&acao=C&usucpf="+ usucpf;
        <?php endif; ?>
    }

    function alterar_usu( usucpf ){
        <?php if ( $_SESSION['sisid'] == 147 ): ?>
        window.location = "<?=$_SESSION['sisarquivo']?>.php?modulo=sistema2/usuario2/cadusuario&acao=A&usucpf="+ usucpf;
        <?php else: ?>
        window.location = "<?=$_SESSION['sisarquivo']?>.php?modulo=sistema/usuario/cadusuario&acao=A&usucpf="+ usucpf;
        <?php endif; ?>
    }

    function SelPerfil(){
        if ( document.formulario.pflcod.value == '' ) {
            alert('Selecione o Perfil antes de escolher a Situação Perfil.');
            document.formulario.sitperfil[0].checked = false;
            document.formulario.sitperfil[1].checked = false;
            document.formulario.sitperfil[2].checked = false;
        }
    }

    --></script>