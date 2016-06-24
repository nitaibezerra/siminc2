<?php
# carrega as bibliotecas
include_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
include_once APPRAIZ . "includes/PogProgressBar.php";
include_once APPRAIZ . "includes/envia_email_sis_geral_funcoes.inc";
include_once APPRAIZ . "includes/Agrupador.php";
require_once APPRAIZ . "includes/funcoesspo_componentes.php";
include_once APPRAIZ . 'includes/SpoComunicados/classes/Comunicado.php';
require_once APPRAIZ . 'includes/library/simec/Listagem.php';

$comunicado = new Comunicado(MODULO);


if($_REQUEST['formSelecaoUsuario'] == true){
    #enviaEmailSelecaoUsuarios();
    $comunicado->enviaEmail();
    die();
}

if($_REQUEST['enviaEmailParaUsuarios'] == true){
    #enviaEmailParaUsuarios();
    $comunicado->DisparaEmail();
    die();
}

include APPRAIZ."includes/cabecalho.inc";

?>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/_start.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/keys/keyEvents.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/tags/suggest.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
<script language="javascript" type="text/javascript">

    function submeterEnvioEmail(){
        var dadosListaUsuarios = $('#formularioListaUsuarios').serializeArray();
        var dadosEmailEnvio    = $('#formulario').serializeArray();

        var mensagem          = '';
        var assunto           = '';
        var stNomeRemetente   = '';
        var stEmailRemetente  = '';
        var obj               = {};
        var arrListaUsuarios  = [];
        var arrUsuario; //Temp

        for(var c = 0; c < dadosListaUsuarios.length; c++){
            obj = dadosListaUsuarios[c];
            if(typeof(obj.name) != 'undefined'){
                if(obj.name == 'data[id]'){
                    arrUsuario = obj.value.split(',');
                    arrListaUsuarios.push({usucpf: arrUsuario[0], usunome: arrUsuario[1], usuemail: arrUsuario[2]})
                }
            }
        }

        for(var c = 0; c < dadosEmailEnvio.length; c++){
            obj = dadosEmailEnvio[c];
            if(typeof(obj.name) != 'undefined'){
                if(obj.name == 'mensagem')        { mensagem         = obj.value; }
                if(obj.name == 'assunto')         { assunto          = obj.value; }
                if(obj.name == 'stNomeRemetente') { stNomeRemetente  = obj.value; }
                if(obj.name == 'stEmailRemetente'){ stEmailRemetente = obj.value; }
            }
        }

        var url   = window.location.href+'&enviaEmailParaUsuarios=true&enviar=true';
        var dados = {arDestinatarios: arrListaUsuarios, mensagem: mensagem, assunto: assunto, stNomeRemetente: stNomeRemetente, stEmailRemetente: stEmailRemetente };

        $.ajax({
                url: url,
                type: "POST",
                data: dados,
                success: function(data){
                    console.log(data);
                    bootbox.alert(' '+data+' ',function(){
                        window.location.href = window.location.href;
                    });
                }
            });

    }

    function submeter_formulario(){

        var formulario = $('#formulario');
        var perfil = formulario.perfil;
        if ( perfil ) {
            selectAllOptions( perfil );
        }

        $('input [name=formulario]').val(2);

        var dadosForm = $( '#formulario' ).serialize();

        $.ajax({
            url: window.location.href+'&formSelecaoUsuario=true&'+dadosForm,
            success: function(html) {
                $('#modal-confirm .modal-body p').html(html);
                $('.modal-dialog').css('width', '90%');
                $('#modal-confirm .modal-title').html('Comunicado - Disparo de Emails');
                $('#modal-confirm .btn-primary').html('Salvar');
                $('#modal-confirm .btn-primary').attr('type','button');
                $('#modal-confirm .btn-primary').attr('name','enviar');
                $('#modal-confirm .btn-primary').attr('value','enviar');
                $('#modal-confirm .btn-primary').attr('onclick','submeterEnvioEmail();');
                $('#modal-confirm .btn-default').html('Fechar');
                $('.modal-dialog').show();
                $('#modal-confirm').modal();
            }
        });

    }

    function retornar(){
        window.location = $('#modulo').val() +'.php?modulo=principal/comunicado/listar&acao=A';
    }



</script>
<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo utf8_decode($_SESSION['sisdiretorio']); ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li>Comunicado</li>
        <li class="active">Enviar E-mail</li>
    </ol>

    <section class="well">
        <input type="hidden" id="modulo" value="<?php echo MODULO; ?>" />
        <form id="formulario" method="post" name="formulario" enctype="multipart/form-data" class="form-horizontal">
            <input type="hidden" name="formulario" value="1"/>
            <input type="hidden" name="usustatus" value="A"/>
            <section class="form-group">
                <label for="mnucod" class="col-md-2 control-label">Perfil:</label>
                <section class="col-md-10">
                <?php
                    $sql = <<<DML
                        SELECT
                            pflcod as codigo,
                            pfldsc as descricao
                        FROM perfil
                        WHERE sisid = '{$_SESSION["sisid"]}'
                            AND pflstatus = 'A'
                        ORDER BY pfldsc
DML;
                    $options = array('titulo' => 'Selecione o(s) Perfil(s)', 'multiple' => 'multiple', 'size' => '10');
                    inputCombo('perfil[]', $sql, $dados['colunas'], 'perfil[]', $options);
                    $assunto = 'NOVO Comunicado Cadastrado';
                ?>
                </section>
            </section>
            <section class="form-group">
                <label for="mnucod" class="col-md-2 control-label">Assunto:</label>
                <section class="col-md-10">
                    <?php echo inputTexto('assunto', $assunto, 'assunto', 50, false, array()) ?>
                </section>
            </section>
            <?php
            if(!empty($_GET['arqid'])){
                //recuperando descricão do comunicado
                $descricao = $comunicado->buscarDescricao($_GET['arqid']);
            }else{
                echo <<<HTML
                    <script>
                        alert('Arquivo não encontrado');
                        window.location = $('#modulo').val() +'.php?modulo=principal/comunicado/listar&acao=A';
                    </script>
HTML;
            }
            $html = <<<HTML
Prezado(a),<br>
O comunicado {$descricao} foi cadastrado no SIMEC, no módulo {$_SESSION['sisdsc']}.<br>
Acesse o SIMEC para baixar o arquivo.<br><br>

Atenciosamente,<br><br>

Equipe SIMEC<br>
simec.mec.gov.br<br><br>

-- ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. --
HTML;
            ?>
            <section class="form-group control-group">
                <label for="mnucod" class="col-md-2 control-label">Mensagem:</label>
                <section class="col-md-10">
                    <textarea id="mensagem" name="mensagem" rows="15" cols="100" class=""><?= $html ?></textarea>
                </section>
            </section>

            <input class="normal" type="hidden" value="SIMEC" id="stNomeRemetente" name="stNomeRemetente"/>
            <input class="normal" type="hidden" value=$_SESSION['email_sistema'] name="stEmailRemetente" id="stEmailRemetente" />

            <section class="form-group">
                <section class="col-md-2">
                </section>
                <section class="col-md-10">
                    <input class="btn btn-warning" id="voltar" type="button" value="Voltar" onclick="retornar();" />
                    <button type="button" class="btn btn-success" onclick="submeter_formulario();"><span class="glyphicon glyphicon-share-alt"></span> Enviar</button>
                </section>
            </section>

        </form>
    </section>
</section>

