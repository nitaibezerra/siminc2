<?php

include APPRAIZ."monitoraseb/classes/SubacaoCoordenacao.class.inc";
$oSubacaoCoordenacao = new SubacaoCoordenacao();
if($_REQUEST['executa'] && $_REQUEST['executa'] == 'salvar'){
    $erros = $oSubacaoCoordenacao->salvar($_REQUEST['cooid'],$_POST['arraySubAcao'],$_POST['arrayCombos']);

    if(empty($erros)){
        echo '<script>
                alert(\'Operação realizada com sucesso!\');
                window.parent.opener.location.reload();
                self.close();
              </script>';
    }else{
        echo '<script>
                alert(\''.$erros.'!\');
              </script>';
    }
}

?>
<html>
<head>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

    <link rel='stylesheet' type='text/css' href='../includes/superTitle.css'/>
    <script type="text/javascript" src="../includes/remedial.js"></script>
    <script type="text/javascript" src="../includes/superTitle.js"></script>

    <script type="text/javascript">

        $(document).ready(function(){
            $("input").not( $(":button") ).keypress(function (evt) {
                if (evt.keyCode == 13) {
                    $('#btbuscar').click();
                    return false;
                }
            });

            $('#botaoOk').click(function(){
                if(validaFormulario()){
                    $('#formulario').submit();
                }
            });

            jQuery('.checkAssociado').mousemove(function(){
                    SuperTitleOff( this );
                    if($(this).find('img')[0]){
                        SuperTitleOn( this,$(this).find('img').attr('alt'));
                    }
                })
                .mouseout(function(){
                    if($(this).find('img')[0]){
                        SuperTitleOff( this );
                    }
            });

        });

        function habilitaCombo(check){
            if(check.checked){
                var combo = document.getElementById("combo_"+check.value);
                combo.disabled = false;
                combo.className = 'CampoEstilo obrigatorio';
            }else{
                var combo = document.getElementById("combo_"+check.value);
                combo.disabled = true;
                combo.className = 'disabled';
            }
        }

        function validaFormulario(){
            var erro = 0;
            $("[class~=obrigatorio]").each(function() {
                if(!this.value){
                    erro = 1;
                    alert('Favor selecionar o Coordenador Técnico para todas as Subações marcadas!');
                    this.focus();
                    return false;
                }
            });
            if(erro == 0){
                return true;
            }
        }

    </script>
</head>
<body>
<?php

if($chkEscolas == 2){
    $ckNaoAssociadas = "checked=\"checked\"";
} else {
    $ckTodas = "checked=\"checked\"";
}
//ver($tecid,d);
?>
<form name="formulario" id="formulario" method="post" action="?modulo=principal/equipegestao/popup/selecionarsubacao&acao=A">
<?php
echo $oSubacaoCoordenacao->montaListaSubacao($_REQUEST['cooid']);
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="3" align="center" >
    <tr bgcolor="#cccccc">
        <td width="9px"></td>
        <td align="right">
            <input type="button" class="botao" id="botaoOk" name="bta" value="Salvar">
            <input type="hidden" id="cooid" name="cooid" value="<?=$_REQUEST['cooid']?>">
            <input type="hidden" id="executa" name="executa" value="salvar">
        </td>
    </tr>
</table>
</form>
</body>
</html>