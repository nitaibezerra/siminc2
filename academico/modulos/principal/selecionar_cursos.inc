<?php
    if ($_REQUEST['exec_function']) {
        header('Content-Type: text/html; charset=ISO-8859-1');
        $_REQUEST['exec_function']($_REQUEST);
    }
    
    $sql = "
        SELECT teu.orgid 
        FROM academico.campus cam 
        INNER JOIN entidade.entidade e ON e.entid = cam.entid 		    
        INNER JOIN entidade.funcaoentidade fe ON fe.entid = e.entid
        INNER JOIN entidade.funentassoc fea ON fea.fueid = fe.fueid
        INNER JOIN entidade.entidade ea ON ea.entid = fea.entid 
        INNER JOIN entidade.funcaoentidade fe1 ON fe1.entid = ea.entid
        INNER JOIN academico.orgaouo teu ON teu.funid = fe1.funid 
        WHERE cam.cmpid = '".$_REQUEST['cmpid']."'
    ";
    $orgid = $db->pegaUm($sql); 
?>

<html>
    <head>
        <title>Inserir Cursos</title>
        
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
        
        <script language="JavaScript" src="../../includes/funcoes.js"></script>
        <script language="JavaScript" src="../includes/prototype.js"></script>
        <script language="JavaScript" src="geral/js/academico.js"></script>
        
        <script>
            function crtcursos(chk) {
                var orgid = <? echo $orgid; ?>;
                var cmpid = <? echo $_REQUEST['cmpid']; ?>;
                var cpcprevisto = document.getElementById('cpcprevisto').value;

                if(chk.checked) {
                    ajaxatualizar('exec_function=inserircampuscurso&orgid='+orgid+'&curid='+chk.value+'&cpcprevisto='+ cpcprevisto ,'tabela_cursos');
                    window.opener.ajaxatualizar('exec_function=carregarvagasporcurso&orgid=' + orgid + '&cmpid=' + cmpid + '&cpcprevisto=' + cpcprevisto, 'tabela_cursos');
                }else{
                    var conf = confirm("Essa ação irá remover todos os dados vinculados ao curso neste campus. Deseja continuar?");
                    if(conf){
                        ajaxatualizar('exec_function=removercampuscurso&curid=' + chk.value + '&cpcprevisto=' + cpcprevisto, '');
                        window.opener.ajaxatualizar('exec_function=carregarvagasporcurso&orgid=' + orgid + '&cmpid=' + cmpid, 'tabela_cursos');
                    }
                }
            }

            function selecionatipocurso() {
                var tpcid = document.getElementById('unidades').value;
                var cpcprevisto = document.getElementById('cpcprevisto').value;
                var curdsc = document.getElementById('n_curdscp').value;

                if (tpcid && cpcprevisto) {
                    ajaxatualizar('exec_function=listarcursos&cmpid=<? echo $_REQUEST['cmpid']; ?>&curdsc=' + curdsc + '&tpcid=' + tpcid + '&cpcprevisto=' + cpcprevisto, 'listacurso');
                    document.getElementById('linhalistacurso').style.display = '';
                    exibeInserirNovoCurso(tpcid, cpcprevisto);

                }else{
                    alert("Selecione o tipo e informe previsto/realizado");
                    document.getElementById('linhalistacurso').style.display = 'none';
                    document.getElementById('listacurso').innerHTML = '';

                    document.getElementById('inserir_curso1').style.display = 'none';
                    document.getElementById('inserir_curso2').style.display = 'none';
                    document.getElementById('inserir_curso3').style.display = 'none';
                    document.getElementById('inserir_curso4').style.display = 'none';
                }
            }
        </script>
    </head>
    <body>
        <form id="n_form" name="n_form" method="post" action="" >
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
                <tr>
                    <td	 class="SubTituloDireita">Tipo de curso :</td>
                    <td>
                        <?
                        $sql = "SELECT tpc.tpcid as codigo, tpc.tpcdsc as descricao, teu.orgid, tpc.tpcedicao
					    FROM academico.campus cam 
					    INNER JOIN entidade.entidade e ON e.entid = cam.entid
					    INNER JOIN entidade.funcaoentidade fe ON fe.entid = e.entid
					    INNER JOIN entidade.funentassoc fea ON fea.fueid = fe.fueid
					    INNER JOIN entidade.entidade ea ON ea.entid = fea.entid
			 		    INNER JOIN entidade.funcaoentidade fe1 ON fe1.entid = ea.entid
					    INNER JOIN academico.orgaouo teu ON teu.funid = fe1.funid 		    
					    INNER JOIN public.tipocurso tpc ON tpc.tpeid = teu.orgid 
					    WHERE cam.cmpid = '" . $_REQUEST['cmpid'] . "'";

                        $db->monta_combo('unidade', $sql, 'S', 'Selecione', '', '', '', '150', 'S', 'unidades', '', $_POST['unidade']);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td	 class="SubTituloDireita">Previsto / Realizado :</td>
                    <td>
                        <select id="cpcprevisto" onchange="" style="width: 150px;" class="CampoEstilo obrigatorio" name="cpcprevisto">
                            <option value="">Selecione</option>
                            <option value="true">Previsto</option>
                            <option value="false">Realizado</option>
                        </select>
                    </td>
                </tr>
                <tr >
                    <td	 class="SubTituloDireita">Descrição :</td>
                    <td>
                        <?php
                        echo campo_texto('n_curdscp', 'N', 'S', '', 30, 40, '', '', "", "", "", "id='n_curdscp'", "");
                        ?>
                        <input type=button value="Pesquisar" onclick="selecionatipocurso();">
                    </td>
                </tr>
                <tr id="linhalistacurso" style="display:none">
                    <td colspan="2" id="listacurso"></td>
                </tr>
            </table>
        </form>
        <script type="text/javascript" src="../includes/remedial.js"></script>
        <script language="JavaScript" src="../includes/wz_tooltip.js"></script>

        <? if ($_POST['unidade']): ?>
            <script>
                selecionatipocurso(<?= $_POST['unidade'] ?>);
            </script>
        <? endif; ?>  
    </body>
</html>