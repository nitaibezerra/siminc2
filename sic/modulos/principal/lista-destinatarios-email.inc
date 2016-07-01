<?php

//ver($_SESSION['sisid'],d);
//ver($_REQUEST,d);
if($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}

function adicionarUsuario(){
    global $db;
    
    $cpf = $_REQUEST['cpf'];
    $sql = "
        INSERT INTO 
          sic.usuarioenvioemail
        (
          usucpf,
          ueestatus
        ) 
        VALUES (
          '$cpf',
          'A'
        )
    ";

    $db->executar($sql);
    $db->commit();
    $msgSucesso = "
        <script>
            alert('Usuário cadastrado com sucesso!');
            window.location.href = 'sic.php?modulo=principal/lista-destinatarios-email&acao=A';
        </script>
    ";
    echo $msgSucesso;
}

function excluirDestinatario(){
    global $db;
    
    $ueeid = $_REQUEST['ueeid'];
    $sql = "
        DELETE FROM 
          sic.usuarioenvioemail 
        WHERE 
          ueeid = $ueeid
    ";
    $db->executar($sql);
    $db->commit();
    $msgSucesso = "
        <script>
            alert('Usuário excluído com sucesso!');
            window.location.href = 'sic.php?modulo=principal/lista-destinatarios-email&acao=A';
        </script>
    ";
    echo $msgSucesso;
}

function exibirTelaAdicionar(){
    global $db;
    ?>
<form id="formUsuarios" name="formUsuarios" method="POST" action="javascript:;">
    	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
            <tr>
                <td class="SubTituloDireita">CPF</td>
                <td>
                    <?php echo campo_texto('txtFormUsuariosCpf', 'S', 'S', 'CPF', 16, 14, "###.###.###-##", "", '', '', 0, 'id="txtFormUsuariosCpf"', '', mascaraglobal(str_replace(array('.','-'), '', @$_REQUEST['txtCpf']),"###.###.###-##") ); ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Nome</td>
                <td>
                    <?php echo campo_texto('txtFormUsuariosNome', 'S', 'S', 'Nome', 8, 7, "", "", '', '', 0, 'id="txtFormUsuariosNome"', '',@$_REQUEST['txtNome'] ); ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Perfil</td>
                <td>
                    <?php $db->monta_combo("selFormUsuariosPerfil", retornarSqlBuscarPerfil($_SESSION['sisid']),'S',"Selecione...",'', '', '','','S','selFormUsuariosPerfil'); ?>
                </td>
            </tr>
            <tr>
                <td id="td_filtros" colspan="2">
                    <input type="button" id="btnFechar" value="Fechar">
                    <input type="button" id="btnLimparUsuario" value="Limpar">
                    <input type="submit" id="btnPesquisarUsuario" value="Pesquisar">
                </td>
            </tr>
    	</table>
    </form>
    <div style="margin: 0 auto; padding: 0; height: 430px; width: 100%; background-color: #eee; border: none;" class="div_rolagem" id="div_usuarios"></div>
    <script>
        jQuery(document).ready(function() {
            jQuery('#formUsuarios').submit( function(){
                var cpf = jQuery('#txtFormUsuariosCpf').val();
                var nome = jQuery('#txtFormUsuariosNome').val();
                var perfil = jQuery('#selFormUsuariosPerfil').val();
                pesquisarUsuarios(cpf, nome, perfil);
                return false;
            });
            
            jQuery('#btnLimparUsuario').click(function(){
                jQuery('#txtFormUsuariosCpf').val('');
                jQuery('#txtFormUsuariosNome').val('');
                jQuery('#selFormUsuariosPerfil').val('');
            });
            
            jQuery('#btnFechar').click(function(){
                closeMessage();
            });
            
        });
    </script>
    <?php
}

function pesquisarUsuarios(){
    global $db;
    
    $where = "";
    $cpf = str_replace(array('.','-'), '', $_REQUEST['cpf']);
    if(!empty($cpf)) $where .= " AND usuario.usucpf = '$cpf'";
    $nome = $_REQUEST['nome'];
    if(!empty($nome)) $where .= " AND usuario.usunome ILIKE '%$nome%' ";
    $perfil = $_REQUEST['perfil'];
    if(!empty($perfil)) $where .= " AND perfil.pflcod = $perfil ";

    if(!empty($perfil)) {
        $join_sql = sprintf( "
                seguranca.perfil perfil 
                JOIN seguranca.perfilusuario perfilusuario ON perfil.pflcod = perfilusuario.pflcod AND perfil.pflcod = %d
                RIGHT JOIN seguranca.usuario usuario ON usuario.usucpf = perfilusuario.usucpf
                LEFT JOIN
                (
                  SELECT 
                        unicod, 
                        unidsc
                  FROM 
                        public.unidade
                  WHERE 
                        unitpocod = 'U'
                ) AS unidadex ON usuario.unicod = unidadex.unicod				
            ", $perfil);
    } else {
        $join_sql = "
           (
              SELECT 
                    unicod, 
                    unidsc
              FROM 
                    public.unidade
              WHERE 
                    unitpocod = 'U'
            ) AS unidadex
            RIGHT JOIN seguranca.usuario usuario ON usuario.unicod = unidadex.unicod
            LEFT JOIN seguranca.perfilusuario perfilusuario ON usuario.usucpf = perfilusuario.usucpf
            LEFT JOIN seguranca.perfil perfil ON perfil.pflcod = perfilusuario.pflcod 
        ";
    }

    $adicionar = "<img src=../imagens/alterar.gif style=cursor:pointer; onclick=adicionarUsuario(\''|| usuario.usucpf ||'\') >";
    
    $sqlUsuarios = "
        SELECT DISTINCT
            '<center>$adicionar</center>' AS acao,
            substring(usuario.usucpf from 1 for 3) || '.' || substring (usuario.usucpf from 4 for 3) || '.' || substring (usuario.usucpf from 7 for 3) || '-' || substring (usuario.usucpf from 10 for 2) AS cpf,
            usuario.usunome AS nome,
            usuario.usuemail AS email
        FROM 
            " . $join_sql . "
        LEFT JOIN  territorios.municipio municipio ON municipio.muncod = usuario.muncod
        JOIN seguranca.usuario_sistema usuariosistema ON usuario.usucpf = usuariosistema.usucpf
        LEFT JOIN  entidade.entidade entidade ON usuario.entid = entidade.entid
        LEFT JOIN  public.cargo cargo ON cargo.carid = usuario.carid
        $innerResponsabilidade	
        WHERE 
            usunome IS NOT NULL
            AND usuariosistema.suscod = 'A'
            $where
            AND usuariosistema.sisid = ".$_SESSION['sisid'];

    $cabecalho = array("Ação", "CPF", "Nome", "Email");
    $db->monta_lista_simples($sqlUsuarios, $cabecalho, 100, 5, 'N', '70%', '', '', '', '', 't');
    die;
}

include APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";
?>
<!--	<script language="javascript" type="text/javascript" src="./js/projovemcampo.js"></script>-->
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />

<form id="form" name="form" method="POST">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="SubTituloCentro" colspan="2">Destinatarios de Email</td>
        </tr>
        <tr>
            <td class="SubTituloDireita">CPF</td>
            <td>
                <?php echo campo_texto('txtCpf', 'S', 'S', 'CPF', 16, 14, "###.###.###-##", "", '', '', 0, 'id="txtCpf"', '', mascaraglobal(str_replace(array('.','-'), '', @$_REQUEST['txtCpf']),"###.###.###-##") ); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Nome</td>
            <td><?php echo campo_texto('txtNome', 'S', 'S', 'Nome', 8, 7, "", "", '', '', 0, 'id="txtNome"', '',@$_REQUEST['txtNome'] ); ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">E-mail</td>
            <td><?php echo campo_texto('txtEmail', 'S', 'S', 'E-mail', 8, 7, "", "", '', '', 0, 'id="txtEmail"', '', @$_REQUEST['txtEmail'] ); ?></td>
        </tr>
        <tr>
            <td class="SubTituloCentro" colspan="2">
                <input type="button" id="btnAdicionar" value="Adicionar">
                <input type="button" id="btnLimpar" value="Limpar">
                <input type="submit" id="btnPesquisar" value="Pesquisar">
            </td>
        </tr>
    </table>
</form>
<?php
monta_titulo('Usuários Cadastrados para Recebimento do Email', '');

$deletar = "<img src=../imagens/excluir.gif style=cursor:pointer; onclick=excluirDestinatario(\'sic.php?modulo=principal/lista-destinatarios-email&acao=A&requisicao=excluirDestinatario&ueeid='|| use.ueeid ||'\') >";

# Filtros da pesquisa
$cpf = str_replace(array('.','-'), '', @$_REQUEST['txtCpf']);
if(!empty($cpf)) $where = " AND us.usucpf = '$cpf' ";
$nome = $_REQUEST['txtNome'];
if(!empty($nome)) $where .= " AND us.usunome ILIKE '%$nome%'";
$email = $_REQUEST['txtEmail'];
if(!empty($email)) $where .= " AND us.usuemail ILIKE '%$email%' ";

$sql = "
    SELECT DISTINCT
        '<center> $deletar <center>' AS acao,
        substring(us.usucpf from 1 for 3) || '.' || substring (us.usucpf from 4 for 3) || '.' || substring (us.usucpf from 7 for 3) || '-' || substring (us.usucpf from 10 for 2) AS cpf,
        us.usunome AS nome,
        us.usuemail AS email
    FROM
        sic.usuarioenvioemail use
        JOIN seguranca.usuario us ON use.usucpf = us.usucpf
    WHERE
        use.ueestatus = 'A'
        $where
";
//ver($sql,d);
$cabecalho = array("Ação", "CPF", "Nome", "Email");
$db->monta_lista_simples($sql, $cabecalho, 100, 5, 'N', '100%', '', '', '', '', 't');
?>
<script type="text/javascript">
    
    jQuery(document).ready(function() {
        jQuery('#btnAdicionar').click(function(){
            abrirPopupUsuários();
        });
        
        jQuery('#btnLimpar').click(function(){
            jQuery('input[type=text]').val('');
        });
        
    });
    
    function abrirPopupUsuários(){
        var today = new Date();
        displayMessage('sic.php?modulo=principal/lista-destinatarios-email&acao=A&requisicao=exibirTelaAdicionar&nocache='+today);
    }
    
    function pesquisarUsuarios(cpf, nome, perfil){
        jQuery.ajax({
            type: "POST",
            url: "sic.php?modulo=principal/lista-destinatarios-email&acao=A",
            data: "requisicao=pesquisarUsuarios&cpf="+cpf+"&nome="+nome+"&perfil="+perfil,
            async: false,
            success: function(msg){
                document.getElementById('div_usuarios').innerHTML=msg;
            }
        });
    }
    
    function adicionarUsuario(cpf) {
        window.location.href = 'sic.php?modulo=principal/lista-destinatarios-email&acao=A&requisicao=adicionarUsuario&cpf='+cpf;
    }
    
    function excluirDestinatario(ueeid) {
        window.location.href = 'sic.php?modulo=principal/lista-destinatarios-email&acao=A&requisicao=excluirDestinatario&ueeid='+ueeid;
    }
    
    messageObj = new DHTML_modalMessage();	// We only create one object of this class
    messageObj.setShadowOffset(5);	// Large shadow

    function displayMessage(url) {
        messageObj.setSource(url);
        messageObj.setCssClassMessageBox(false);
        messageObj.setSize(800, 600);
        messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
        messageObj.display();
    }

    function closeMessage() {
        messageObj.close();
    }
</script>

