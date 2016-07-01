<?php
// inicializa sistema
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "www/conjur/_constantes.php";
$db = new cls_banco();


switch ( $_REQUEST['status'] ) {
    case 'A':
    case 'I':
        $PstStatus = $_REQUEST['status'];
        break;
    case 'T':
        $PstStatus = '';
        break;
    default:
        $PstStatus = 'A';
        break;
}

$sqlListaUsuarios =
"SELECT DISTINCT u.usucpf AS codigo,
       u.usunome AS descricao 
       
FROM seguranca.usuario u
INNER JOIN conjur.usuarioresponsabilidade r ON r.usucpf = u.usucpf

INNER JOIN seguranca.usuario_sistema as us on us.usucpf = u.usucpf
WHERE r.coonid = 4
  AND rpustatus = 'A'
  AND us.suscod = 'A' 
  AND EXISTS
    ( SELECT 1
     FROM seguranca.perfilusuario
     WHERE usucpf = u.usucpf
       AND pflcod = 244 )

       and u.usucpf not in ( SELECT
								 usucpf
								FROM
								 seguranca.usuario_sistema us
								 LEFT JOIN seguranca.perfil  p USING ( pflcod )
								WHERE
								 us.sisid = 29 AND
								 usucpf = u.usucpf AND us.suscod in ('P','B') )
ORDER BY u.usunome
		";
// ver($sqlListaAdvogados,d);
$listaUsuarios = $db->carregar( $sqlListaUsuarios );

?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<form id="formulario" name="formulario" method="post" action="">
        <table width="100%" bordercolor="#DCDCDC">
            <tr><td style="background-color:#e9e9e9"><b>Selecione:</b></td><td style="background-color:#e9e9e9"><b>Usuários</b></td></tr>
            <?
            if(is_array($listaUsuarios)){
                foreach($listaUsuarios as $usuarios):
                    $Cor = "#e2e6e7 ";
                    $Divisao=$Cont%2;
                    ($Divisao == 0)? $Cor = "#fbfbfb " : "";
                    $Cont = $Cont+1;

                    ?>
                    <tr>
                        <td style="background-color:<?=$Cor?>;">
                            <input name="checkbox<?= $usuarios['codigo']?>" type="checkbox" title="<?= $usuarios['descricao']?>" id="checkbox<?= $usuarios['codigo']?>" value="<?= $usuarios['codigo']?>"
                                   onclick="obterMarcados('checkbox<?= $usuarios['codigo']?>', '<?= $usuarios['codigo']?>','<?= str_replace("'","",$usuarios['descricao'])?>');" />
                        </td>
                        <td  style="background-color:<?=$Cor?>;"><?=$usuarios['descricao']?> </td>

                    </tr>
                <?
                endforeach;
                ?>
                <tr>
                    <td style="background-color:#e9e9e9">
                        <b> Total (
                            <script>
                                var x=document.forms.formulario.status
                                window.document.write(x.options[x.selectedIndex].text)
                            </script>):
                        </b>
                    </td>
                    <td style="background-color:#e9e9e9"><b><?=$Cont?></b></td>
                </tr>
                <tr>
                    <td>
                        <input id="marcaDesmarcaTodos" type="checkbox" name="marcaDesmarcaTodos" value="marcaDesmarcaTodos" onclick="marcarDesmarcarTodos(this);"  /> Marcar / Desmarcar Todos
                    </td>
                    <td>
                        <input type="button" onclick="self.close();" value="Ok" name="ok">
                    </td>
                </tr>
            <?
            }else{
                echo "<tr><td>";
                echo "Sem registros";
                echo "</td></tr>";
            }
            ?>
        </table>
    <div>


    </div>
</form>
<script language="javascript">
    var k = 0;
    var t = opener.document.formulario.usuid;
    var a = document.formulario.elements;
    for(k; k< a.length; k++){
        var elementoatual = a[k];
        switch(elementoatual.type){
            case "checkbox":
                for(i=0;i<t.length;i++){
                    var item = t.options[i];
                    if(item.value == elementoatual.value){
                        elementoatual.checked = true;
                    }
                }
                break;
            default:
                continue;
                break;
        }
    }

    function obterMarcados(Nome,Valor,Descricao) {
        checkBox = document.getElementById(Nome);
        if ( checkBox.checked && checkBox.id != 'marcaDesmarcaTodos' ) {
            if((opener.document.formulario.usuid.options.length == 1) && (opener.document.formulario.usuid.options[0].value == "")){
                opener.document.formulario.usuid.options[0] = null;
            }
            var d=opener.document.formulario.usuid.options.length++;
            opener.document.formulario.usuid.options[d].text = Descricao;
            opener.document.formulario.usuid.options[d].name = Nome;
            opener.document.formulario.usuid.options[d].value = Valor;
            opener.document.formulario.usuid.options[d].setAttribute("selected","selected");
        }else{
            var listaOpcoes = opener.document.formulario.usuid.options;
            for(x = 0 ; x< listaOpcoes.length; x++){
                if(listaOpcoes[x].value == Valor ){
                    opener.document.formulario.usuid.options[x] = null;
                }
                if(listaOpcoes.length == 0){
                    var textocombogeral = "Duplo clique para selecionar da lista";
                    var d=opener.document.formulario.usuid.options.length++;
                    opener.document.formulario.usuid.options[d].text = textocombogeral;
                    opener.document.formulario.usuid.options[d].value = "";
                    //opener.document.formulario.usuid.options[d].setAttribute("","");
                }
            }
        }
    }

    function marcarDesmarcarTodos(checkbox){
        for (i=0;i<document.formulario.elements.length;i++){
            if(document.formulario.elements[i].type == "checkbox"){
                if(checkbox.checked == true){
                    document.formulario.elements[i].checked=true;
                } else {
                    document.formulario.elements[i].checked=false;
                }
                Nome = document.formulario.elements[i].name;
                Valor = document.formulario.elements[i].value;
                Descricao = document.formulario.elements[i].title;
                obterMarcados( Nome, Valor, Descricao);
            }
        }
    }

    function selecionaTodos(){
        for (i=0;i<document.formulario.elements.length;i++){
            if(document.formulario.elements[i].type == "checkbox"){
                document.formulario.elements[i].checked=true;
                Nome = document.formulario.elements[i].name;
                Valor = document.formulario.elements[i].value;
                //Estado = document.formulario.elements[i].title;
                obterMarcados( Nome, Valor);
            }
        }
    }

</script>