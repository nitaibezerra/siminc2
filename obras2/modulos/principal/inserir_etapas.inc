<?php

function podeExcluir($itcid) {
//    $sql_excluir = pg_query("SELECT
//                                count(*) as num
//                             FROM
//                                obras.itenscomposicaoobra itco
//                             WHERE
//                                itco.obrid = ".$_SESSION["obra"]["obrid"]."
//                                AND itco.itcid = ".$itcid."
//                                AND (itco.icovlritem is not null
//                                OR itco.icopercprojperiodo is not null) AND
//                                icostatus = 'A'");
//    $dados = pg_fetch_array($sql_excluir);

//    return $dados["num"];
    return 0;
}

function montaPopupEtapas(){
    $sql = pg_query("SELECT DISTINCT itcid, itcdesc, itcdescservico FROM obras2.itenscomposicao WHERE itcstatus = 'A' ORDER BY itcdesc");
    $count = "1";
    while (($dados = pg_fetch_array($sql)) != false){
        $itcid 			= $dados['itcid'];
        $itcdesc 		= addslashes($dados['itcdesc']);
        $itcdescservico = addslashes($dados['itcdescservico']);
        $cor 			= "#f4f4f4";
        $nome 			= "etapa_".$itcid;
        $count++;
        if ($count % 2){
            $cor = "#e0e0e0";
        }

        if (trim($itcdescservico)!='')
        $title = "onmouseover=\"return escape('$itcdescservico');\"";
        else
        $title = "";

        if(podeExcluir($itcid))
            $key = "accesskey=\"x\"";
        else
            $key = "";

        echo "
            <script type=\"text/javascript\"> id_etapas.push('$nome'); </script>
            <tr bgcolor=\"$cor\"  onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$cor';\">
                <td $title>
                <input id=\"".$nome."\" name=\"".$itcdesc."\" type=\"checkbox\" value=\"" . $itcid . "\" $key onclick=\"marcaItem('".$itcdesc."', ".$itcid.", '".$nome."', ".podeExcluir($itcid).");\">" . $itcdesc . "
                </td>
            </tr>
        ";
    };
}

?>
<html>
    <head>
        <title>Inserir Serviços</title>
        <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
        <script language="JavaScript" src="../../includes/funcoes.js"></script>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    </head>
    <script type="text/javascript">
        var id_etapas = new Array();

        function selecionaTodos() {
            var i, etapa, descricao, id, nome;

            for(i=0; i<id_etapas.length; i++) {
                etapa = document.getElementById(id_etapas[i]);

                if((document.getElementById("selecionar_todos").checked == true)&&(etapa.checked == false)) {
                    etapa.checked = true;
                    descricao = etapa.name;
                    id = etapa.value;
                    nome = etapa.id;
                    if(etapa.accessKey == "x")
                        podeExcluir = 1;
                    else
                        podeExcluir = 0;

                    marcaItem(descricao, id, nome, podeExcluir);
                } else if((document.getElementById("selecionar_todos").checked == false)&&(etapa.checked == true)) {
                    etapa.checked = false;
                    descricao = etapa.name;
                    id = etapa.value;
                    nome = etapa.id;
                    if(etapa.accessKey == "x")
                        podeExcluir = 1;
                    else
                        podeExcluir = 0;

                    marcaItem(descricao, id, nome, podeExcluir);
                }
            }
        }

        function marcaItem(descricao, id, nome, podeExcluir) {
            if(document.getElementById(nome).checked == true) {
                window.opener.gerenteEdificacao.addLine({itcid:id, itcdesc:descricao});
            }else{
                 if(podeExcluir) {
                     document.getElementById(nome).checked = true;
                     alert("Existe supervisão cadastrada para esta etapa.");
                 }else {
                     window.opener.gerenteEdificacao.removeLine( 'tr_etapa_' + id );
                 }
            }
        }
    </script>
    <body>
        <table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none; width:100%;">
            <tr>
                <td width="100%" align="center">
                    <label class="TituloTela" style="color: #000000;">
                        Inserir Etapas
                    </label>
                </td>
            </tr>
        </table>

        <br/>

        <table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
            <tr bgcolor="#cdcdcd">
                <td colspan="2" valign="top">
                    <strong>Selecione a(s) Serviço(s)</strong>
                </td>
            </tr>
            <tr>
                <td>
                    <input type="checkbox" value="todos" name="selecionar_todos" id="selecionar_todos" onclick="selecionaTodos();"> <strong>Selecionar Todos</strong>
                </td>
            </tr>
            <?php
                montaPopupEtapas();
            ?>
            <tr bgcolor="#C0C0C0">
                <td>
                    <input type="button" name="ok" value="Ok" onclick="self.close();">
                </td>
            </tr>
        </table>
        <script type="text/javascript">
           $('[name*=itcid]', window.opener.document).each(function (i, obj){
                 var itcid = $(obj).val();
                 $('#etapa_' + itcid).attr('checked', true);
               });
        </script>
	    <script type="text/javascript" src="../includes/remedial.js"></script>
	    <script language="JavaScript" src="../includes/wz_tooltip.js"></script>
    </body>
</html>