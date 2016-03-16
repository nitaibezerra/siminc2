<?PHP
    switch( $_REQUEST['evento'] ) {
        case 'excluir':
            if($_GET['pecid']){
                $comp = new AssInt();
                $comp->excluirPEC_G($_GET['pecid']);
            }
        break;
    }

    if ($_REQUEST['req']) {
        $comp->$_REQUEST['req']($_REQUEST);
    }

    extract($_POST);
?>

<script src="/assint/js/assint.js"></script>
<script src="../includes/prototype.js"></script>

<script language="javascript" type="text/javascript">
    function validarIES(){
        if( confirm('Deseja validar todos alunos dessa(s) IES?') ){
            $('req').value = 'validaAlunosIES';
            $('pesqEstudanteDocente').submit();
        }
    }

    function pesquisa(){
        $('tipoPesquisa').value = 'lista';
        $('pesqEstudanteDocente').submit();
    }

    function visualizarXls(){
        $('tipoPesquisa').value = 'xls';
        $('pesqEstudanteDocente').submit();
    }
</script>

<form method="POST" name="pesqEstudanteDocente" id="pesqEstudanteDocente">
    
    <input type="hidden" name="tipoPesquisa" id="tipoPesquisa" value="lista" />
    <input type="hidden" name="req" id="req" value="" />
    
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td colspan="2" class="SubTituloCentro">Filtro de Pesquisa</td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="30%">IES:
            </td>
            <td>	
                <?PHP
                    $sql = "
                        SELECT  co_ies AS codigo, 
                                no_ies AS descricao
                        FROM emec.ies
                        WHERE 1 = 1
                    ";
                    combo_popup('co_ies', $sql, 'IES', '400x400', '', '', '', 'S', '', '', 4, 400, $onpop = null, $onpush = null, $param_conexao = false, Array(Array('codigo' => 'no_ies', 'descricao' => 'IES')), $resp, $mostraPesquisa = true, $campo_busca_descricao = false, "atualizaCurso(this);", $intervalo = false, $arrVisivel = null, $arrOrdem = null);
                ?>
            </td>
        </tr>  
        <tr>
            <td class="SubTituloDireita"> Nome: </td>
            <td><?PHP echo campo_texto('pecnome', 'N', 'S', '', 60, 200, '', ''); ?></td>
        </tr>  
        <tr>
            <td class="SubTituloDireita">País de Origem:</td>
            <td>
                <?PHP
                    $sql = "
                        SELECT  DISTINCT paiid AS codigo, 
                                paidescricao AS descricao 
                        FROM territorios.pais 
                        ORDER BY paidescricao ASC
                    ";
                    $db->monta_combo("paiid", $sql, 'S', "Selecione um País...", '', '','',400);
                ?>
            </td>
        </tr>    
        <tr>
            <td class="SubTituloDireita">Habilitação:</td>
            <td>
                <?PHP
                    $sql = "
                        SELECT  DISTINCT habid AS codigo, 
                                habdesc AS descricao 
                        FROM assint.habilitacao
                        WHERE habstatus = 'A'
                        ORDER BY habdesc ASC
                    ";
                    $db->monta_combo("habid", $sql, 'S', "Selecione uma habilitação...", '', '', '', 400, 'N', '', '', $habid);
                ?>
            </td>
        </tr> 
        <tr>
            <td class="SubTituloDireita">Situação do Aluno:</td>
            <td>
                <?PHP
                    $sql = "
                        SELECT  DISTINCT siaid AS codigo, 
                                siadesc AS descricao 
                        FROM assint.situacaoaluno
                        
                        WHERE siastatus = 'A'
                        ORDER BY siadesc ASC
                    ";
                    $db->monta_combo("siaid", $sql, 'S', "Selecione uma situação...", '', '', '', 400, 'N', '', '', $siaid);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Bolsista:</td>
            <td>
                <?PHP
                    $sql = "
                        SELECT  DISTINCT bolid AS codigo, 
                                boldesc AS descricao 
                        FROM assint.bolsa
                        ORDER BY boldesc
                    ";
                    $db->monta_combo("bolid", $sql, 'S', "Selecione uma modalidade de bolsista...", '', '','',400);
                ?>
            </td>
        </tr>
        <tr>
            <td colspan="2" class="subTituloCentro">
                <input type="button" name="btpesquisar" value="Pesquisar" onclick="pesquisa();" class="botao" />
                <input type="button" name="btcancelar" value="Novo PEC-G" onclick="redireciona('?modulo=principal/cadPEC_G&amp;acao=A');" class="botao" />
                <input type="button" name="btexcel" value="Visualizar XLS" onclick="visualizarXls();" class="botao" />
                
                <?PHP 
                    if (count($co_ies_filtro) > 0){ 
                        echo "<input type=\"button\" name=\"btexcel\" value=\"Validar Alunos Desta IES\" onclick=\"validarIES();\" class=\"botao\"/>";
                        
                        foreach ($co_ies_filtro as $ies){
                            echo "<input type=\"hidden\" name=\"ies[]\" value=\"{$ies}\"/>";
                        }
                    } 
                ?>
            </td>
        </tr>           
    </table>
</form>

<?PHP
    if( !$_POST['tipoPesquisa'] || $_POST['tipoPesquisa'] == 'lista' ){
        $_POST['listaPEC_G'] = true;
        $comp->listaPEC_G($_POST, array('modulo' => 'principal/listaPEC_G'));
    } else {
        ob_clean();
        $_POST['listaPEC_G'] = true;
        $sql = $comp->listaPEC_G($_POST);
        $cab = array("Última Alteração","IES","Natureza da IES","Curso","CPF","Nome","RNE","Matrícula","País","Bolsa","Habilitação","Data Ingresso","Data Conclusão","Rendimento","Situação");
        $db->sql_to_excel($sql,"SIMEC_Relat",$cab);
    }
?>