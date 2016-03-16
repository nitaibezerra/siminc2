<?php
// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
include_once APPRAIZ."sca/classes/Visita.class.inc";
include_once APPRAIZ."sca/classes/Visitante.class.inc";

print '<br/>';

$oVisita = new Visita();
$oVisitante = new Visitante();

$db->cria_aba($abacod_tela,$url,'');
monta_titulo('Registrar baixa de crachá provisório','&nbsp;');

if($_SERVER['REQUEST_METHOD']=="POST"){
    if ($_REQUEST['requisicao'] == 'salvar'){
        $oVisita->confirmarBaixaCracha($_POST['cpsid'], $_SESSION['imagemVisitante']);
        die;
    }
}
?>

<form name="formularioCadastro" id="formularioCadastro" method="post">
    
    <table class="tabela" align="center" cellpadding="2" cellspacing="1" bgcolor="#f5f5f5" width="100%">
        <tr>
            <td>
                <table border="0" cellpadding="2" cellspacing="1" bgcolor="#f5f5f5" width="770">
                    <tr>
                        <th colspan="6">Servidor</th>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="168px">N&uacute;mero do Crach&aacute;:</td>
                        <td width="141px"><?=campo_texto('vstnumcracha','S','S','Informe o número do crachá',20,5,'#####','', 'left', '', 0, 'id="vstnumcracha"','',null,'carregarPessoa(null);');?></td>
                        <td colspan="4"><img id="pesquisa" src="../imagens/consultar.gif" onclick="carregarPessoa(null);" style="cursor: pointer;" ></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Nome:</td>
                        <td colspan="5"><?=campo_texto('vttnome','N','N','',58,100,'','','left','',0,'id="vttnome"');?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Cargo:</td>
                        <td colspan="5"><?=campo_texto('ds_cargo_emprego','N','N','',58,100,'','','left','',0,'id="ds_cargo_emprego"');?></td>
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
                <input style="cursor: pointer;" type="button" name="btnGravar" id="btnGravar" value="Confirmar Baixa" onclick="javascript:confirmarBaixaCracha();">
                <input style="cursor: pointer;" type="button" name="btnLimpar" id="btnLimpar" value="Limpar" onclick="javascript:limparBaixaCracha();">
                <input style="cursor: pointer;" type="button" name="btnVoltar" id="btnVoltar" value="Voltar" onclick="javascript:history.go(-1);">
            </td>
          </tr>
    </table>
    <input type="hidden" name="requisicao" value="salvar" />
    <input type='hidden' id='cpsid' name='cpsid' value='<?=$cpsid?>'>
    <input type='hidden' id='formCarregado' name='formCarregado' value= '0'/>
    <script language='javascript' type='text/javascript' src='./js/sca.js'></script>
<script language="javascript" type="text/javascript" src="./js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript">
   
    function carregarPessoa(vstnumcracha){
        if (vstnumcracha || $('vstnumcracha').getValue() != ''){
            consultarBaixaCracha(vstnumcracha ? vstnumcracha : $('vstnumcracha').getValue());
             //debugger;
        } else{
            limparBaixaCracha();
        }
    }

    function validaBaixaCracha(){

        if($('cpsid').getValue() == '' || $('vstnumcracha').getValue() == '') {
            $('vstnumcracha').focus();
            alert('O campo "Número do Crachá" é obrigatório!');
            $('btnGravar').enable();
            return false;
        }

        return true;
    }

    function confirmarBaixaCracha(){
        $('btnGravar').disable();
        if(validaBaixaCracha()){
            document.formularioCadastro.submit();
        }
    }
</script>
</form>

<script>
    ajustarAbas();

    Event.observe(window, 'load', function() {
        $("vstnumcracha").focus();
      });
</script>