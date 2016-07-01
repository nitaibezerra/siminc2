<?php
// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
include_once APPRAIZ."sca/classes/Visita.class.inc";
include_once APPRAIZ."sca/classes/Visitante.class.inc";

print '<br/>';

$oVisita = new Visita();
$oVisitante = new Visitante();

$db->cria_aba($abacod_tela,$url,'');
monta_titulo('Registrar uso de crachá provisório','&nbsp;');

if($_SERVER['REQUEST_METHOD']=="POST"){
    if ($_REQUEST['requisicao'] == 'salvar'){
        //valida e caso não haja erro, salva
        $mensagemValidacao = $oVisita->confirmarUsoCracha($_POST['nu_matricula_siape'], $_POST['vstnumcracha'], $_SESSION["usucpf"], $_SESSION['imagemVisitante']);
        if(!empty($mensagemValidacao)){
            extract($_POST);
        }
    }
}
?>
<script language='javascript' type='text/javascript' src='./js/sca.js'></script>
<script language="javascript" type="text/javascript" src="./js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript">
    function carregarPessoa(vttid, nu_matricula_siape, nome){
        if (nu_matricula_siape || $('nu_matricula_siape').getValue() != ''){
            consultarEntradaCracha(nu_matricula_siape ? nu_matricula_siape : $('nu_matricula_siape').getValue());
        } else{
            limparEntradaCracha();
        }
    }

    //função que abre em popup a tela de consulta de pessoa
    function consultarPessoa(){
        AbrirPopUp('?modulo=principal/visitante/popup/consultarPessoa&acao=A&tipoConsulta=S', 'consultaPessoas', 'scrollbars=yes, width=1000, height=700');
    }

    function validaUsoCracha(){

        if($('nu_matricula_siape').getValue() == ''){
            $('nu_matricula_siape').focus();
            alert('O campo "Número da Matrícula" é obrigatório!');
            $('btnGravar').enable();
            return false;
        }

        if($('vstnumcracha').getValue() == ''){
            $('vstnumcracha').focus();
            alert('O campo "Número do Crachá" é obrigatório!');
            $('btnGravar').enable();
            return false;
        }

        return true;
    }

    function confirmarUsoCracha(){
        $('btnGravar').disable();
        if(validaUsoCracha()){
            document.formularioCadastro.submit();
        }
    }
</script>
<form name="formularioCadastro" id="formularioCadastro" method="post">
    <table class="tabela" align="center" cellpadding="2" cellspacing="1" bgcolor="#f5f5f5" width="100%">
        <tr>
            <td>
                <table border="0" cellpadding="2" cellspacing="1" bgcolor="#f5f5f5" width="770">
                    <tr>
                        <th colspan="6">Servidor</th>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="168px">Número da matr&iacute;cula:</td>
                        <td width="142px"><?=campo_texto('nu_matricula_siape','S','S','Informe a matrícula do servidor',21,7,'#######','', 'left', '', 0, 'id="nu_matricula_siape"','',null,'carregarPessoa(null, null, null);');?></td>
                        
                        <td colspan="3"><input style="cursor: pointer;" type="button" value="Pesquisa Avançada" name="btnPesquisar" onclick="javascript:consultarPessoa();"></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita">Nome:</td>
                        <td colspan="5"><?=campo_texto('vttnome','N','N','',58,100,'','','left','',0,'id="vttnome"');?></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita">Cargo:</td>
                        <td colspan="5"><?=campo_texto('ds_cargo_emprego','N','N','',58,100,'','','left','',0,'id="ds_cargo_emprego"');?></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita">Lota&ccedil;&atilde;o:</td>
                        <td colspan="5"><?=campo_texto('lotacao','N','N','',58,100,'','','left','',0,'id="lotacao"');?></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita">Sala:</td>
                        <td colspan="5"><?=campo_texto('sala','N','N','',21,100,'','','left','',0,'id="sala"');?></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita">Telefone:</td>
                        <td colspan="5"><?=campo_texto('telefone','N','N','',21,100,'','','left','',0,'id="telefone"');?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" width='170px'>N&uacute;mero do Crach&aacute;:</td>
                        <td colspan="5"><?=campo_texto('vstnumcracha','S','S','Informe o número do crachá',20,5,'#####','', 'left', '', 0, 'id="vstnumcracha"');?></td>
                    </tr>
                </table>
            </td>
            <td width="5%" valign="top">
                <div id="fotoVisitante">
                <?
                if(!empty($vttid) || !empty($nu_matricula_siape)){
                    echo $oVisitante->recuperarFotoVisitante($vttid, $nu_matricula_siape);
                }
                ?>
                </div>
            </td>
        </tr>
        <tr bgcolor="#C0C0C0">
            <td colspan='2' style="text-align:center;">
                <input style="cursor: pointer;" type="button" name="btnGravar" id="btnGravar" value="Confirmar Uso" onclick="javascript:confirmarUsoCracha();">
                <input style="cursor: pointer;" type="button" name="btnLimpar" id="btnLimpar" value="Limpar" onclick="javascript:limparEntradaCracha();">
                <input style="cursor: pointer;" type="button" value="Voltar" name="btnVoltar" id="btnVoltar" onclick="javascript:history.go(-1);">
            </td>
          </tr>
    </table>
    <input type="hidden" name="requisicao" value="salvar" />
</form>
<script>
    ajustarAbas();

    Event.observe(window, 'load', function() {
        $("nu_matricula_siape").focus();
      });
</script>
<?php
//caso tenha ocorrido algum erro na validação
if(!empty($mensagemValidacao)){
    exibeAlerta($mensagemValidacao);
}
?>