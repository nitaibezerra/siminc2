<?PHP
    /*
     * Sistema Simec
     * Desenvolvedor: Equipe SIMEC
     * Analista: Luciano F. Ribeiro.
     * Programador: Luciano F. Ribeiro.
     * Módulo: solicitar.inc
     * Finalidade: permite que usuários com o perfil super-usuário, solicitem acesso ao sistema SIMEC a módulos especificos a usuários do simec do SIMEC.
    */

    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    function enviarSolicitacao( $dados ){
        global $db;

        $sisid = $dados['sisid'];
        $usucpf = str_replace('.', '', str_replace('-', '', $dados['usucpf'] ) );

        $sql = "
            SELECT * FROM seguranca.usuario WHERE usucpf = '{$usucpf}';
        ";
        $usu_cpf = $db->pegaUm($sql);

        if( $usu_cpf == '' ){
            $sql = "
                INSERT INTO seguranca.usuario (usucpf, usunome, usuemail, ususenha, suscod
                    )VALUES(
                        '{$usucpf}', 'Solicitação de Acesso CPF: {$usucpf}. Para Desenvolvimento.', 'teste@teste.com.br', '123456', 'A'
                    ) ;
            ";
            $db->executar($sql);
        }

        if( $usucpf > 0 ){
            $sql = "
                INSERT INTO seguranca.usuario_sistema (usucpf, sisid, suscod) VALUES ('{$usucpf}', {$sisid}, 'P') RETURNING susdataultacesso;
            ";
            $result = $db->pegaUm($sql);
        }

        if( $result != '' ){ 
            $db->commit();
            $db->sucesso("sistema/usuario/solicitar", '&acao=A', "Operação executada com sucesso! Lembre-se que esta operação é realizada para teste de desenvolvimento.");
        }else{
            $db->sucesso("sistema/usuario/solicitar", '&acao=A', "Operação não realizada, Tente novamnete mais tarde!");
        }
    }

    include APPRAIZ . "includes/cabecalho.inc";

    $titulo_modulo='Ficha de Solicitação de Cadastro de Usuários';
    $subtitulo_modulo='Preencha os Dados Abaixo e clique no botão: "Enviar Solicitação"';

    monta_titulo($titulo_modulo,$subtitulo_modulo);
?>

<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="../includes/jquery-autocomplete/jquery.autocomplete.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script	type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>

<script type="text/javascript">

    $1_11(document).ready(function(){
        //#VERSÃO JQUERY INSTANCIADA NO CABEÇALHO
        $1_11(window).unload(
            $1_11('.chosen-select').chosen()
        );
    });

    function atualizarComboMantida( param ){
        if( param == 'D' ){
            $('#tr_combo_mantida').css('display', '');
            $("#mtdid").attr('class', 'obrigatorio');
        }else{
            $('#tr_combo_mantida').css('display', 'none');
            $("#mtdid").attr('class', '');
        }
    }

    function enviarSolicitacao(){
        var erro;
        var radio_entro;
        var check_entro;
        var campos = '';

        $.each($(".obrigatorio"), function(i, v){
            if( ($(this).attr('type') != 'radio') && ($(this).attr('type') != 'checkbox') ){
                if ( $(v).val() == '' ){
                    erro = 1;
                    campos += '- ' + $(this).attr('title') + " \n";
                }
            }else
                if( $(this).attr('type') == 'radio' ){
                    var name  = $(this).attr('name');
                    var value = $(this).attr('value');
                    var radio_box = $('input:radio[name='+name+']:checked');

                    if(!radio_box.val() && (name != radio_entro) ){
                        erro = 1;
                        radio_entro = name;
                        campos += '- ' + $(this).attr('title') + " \n";
                    }
            }else
                if( $(this).attr('type') == 'checkbox' ){
                    var name  = $(this).attr('name');
                    var value = $(this).attr('value');
                    var check_box = $('input:checkbox[name='+name+']:checked');

                    if(!check_box.val() && (name != check_entro) ){
                        erro = 1;
                        check_entro = name;
                        campos += '- ' + $(this).attr('title') + " \n";
                    }
            }
        });

        if( erro > 0 ){
            alert('Existem "campos obrigatorios" vazios! São eles: \n'+campos);
            return false;
        }

        if(!erro){
            $('#requisicao').val('enviarSolicitacao');
            $('#formulario').submit();
        }
    }

    function verificarCPFValido( cpf ){
        var valido = validar_cpf( cpf );
        if(!valido){
            alert( "CPF inválido! Favor informar um CPF válido!" );
            $('#cpdcpf').focus();
            return false;
        }
    }

    function voltarPaginaPrincipal(){
        var janela = window.location.href = 'seguranca.php?modulo=inicio&acao=C';
        janela.focus();
    }

</script>

<form action="" method="POST" id="formulario" name="formulario">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>

    <table align="center" bgcolor="#f5f5f5" border="1" cellpadding="3" cellspacing="1" class="tabela listagem">
	<tr>
            <td class="SubTituloDireita"> Sistema: </td>

            <td>
                <?PHP
                    $sql = "
                        SELECT  s.sisid as codigo,
                                UPPER( s.sisabrev ) AS descricao
                        FROM seguranca.sistema s
                        WHERE s.sisstatus = 'A'
                        ORDER BY 1
                    ";
                    $db->monta_combo('sisid', $sql, 'S', "Selecione...", '', '', '', 390, 'S', 'sisid', '', NULL, 'Sistema', '', 'chosen-select');
                ?>
            </td>
        </tr>
 	<tr>
            <td align='right' class="subtitulodireita">CPF:</td>
            <td>
                <?PHP
                    $usucpf = $dados['usucpf'];
                    echo campo_texto('usucpf', 'S', 'S', 'CPF', 29, 29, '###.###.###-##', '', '', '', 0, 'id="usucpf"', '', $usucpf, 'verificarCPFValido(this.value)', null);
                ?>
            <td>
        </tr>
    </table>

    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela">
        <tr>
            <td width="80%" class="SubTituloCentro" colspan="4" style="font-weight: bold">
                <input type="button" id="salvar" name="enviar" value="Enviar Solicitação" onclick="enviarSolicitacao();"/>
                <input type="button" id="cancelar" name="cancelar" value="Cancelar" onclick="voltarPaginaPrincipal();"/>
            </td>
        </tr>
    </table>
</form>