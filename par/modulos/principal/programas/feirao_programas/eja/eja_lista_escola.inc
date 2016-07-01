<?php

    if( $_SESSION['par']['adpid'] == '' || $_SESSION['par']['prgid'] == '' ){
        $db->sucesso( 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa', '', 'Ocorreu um ploblema. Selecione o programa novamente!' );
        die();
    }

    if($_POST){
        $sql = " DELETE FROM par.pfescolaeja WHERE adpid = {$_SESSION['par']['adpid']} RETURNING ejaid";
	$ejaid = $db->pegaLinha($sql);
	
	if($_POST['entid'][0]){
            foreach($_POST['entid'] as $entid){
                
                $sql = " INSERT INTO par.pfescolaeja(pk_cod_entidade, adpid) VALUES ({$entid}, {$_SESSION['par']['adpid']}) RETURNING ejaid";
                $ejaid = $db->pegaLinha($sql);
            }
	}
	
        if( $ejaid > 0 ){
            $db->commit();
            $db->sucesso('principal/programas/feirao_programas/eja/eja_lista_escola');
        }else{
            $db->insucesso('Não foi possivél gravar o Dados, tente novamente mais tarde!', '', 'principal/programas/feirao_programas/eja/eja_lista_escola&acao=A');
        }
    }

    include APPRAIZ . "includes/cabecalho.inc";
    echo '<br>';

    $sql = "select prgdsc from par.programa where prgid = ".$_SESSION['par']['prgid'];
    $nomePrograma = $db->pegaUm($sql);

    if($_SESSION['par']['muncod']){
        $sql = "select mundescricao, estuf from territorios.municipio where muncod = '{$_SESSION['par']['muncod']}'";
        $rsMunicipio = $db->pegaLinha($sql);
        $descricao = $rsMunicipio['mundescricao'].' - '.$rsMunicipio['estuf'];
    }else{
        $descricao = $_SESSION['par']['estuf'];
    }

    monta_titulo($nomePrograma,$descricao);
    echo '<br>';

    echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/eja/eja_lista_escola&acao=A');

    //permissao edição
    $programa = pegarProgramaDisponivel();
    $programa = $programa ? $programa : array();
    $habilitado = 'S';
    if( !in_array(PROGRAMA_EJA, $programa) ){
            $habilitado = 'N';
    }
?>

<script type="text/javascript">

    function salvar(){
	selectAllOptions( document.getElementById( 'entid' ) );
	document.formulario.submit();
    }

</script>

<form name="formulario" method="post">
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td width="20%" align='right' class="SubTituloDireita">Informe a(s) Escola(s):</td>
            <td>
                <?php
                    if ($_SESSION['par']['itrid'] == 2) {
                        $stWhere = "AND mun.pk_cod_municipio = '{$_SESSION['par']['muncod']}'";
                    } else {
                        $stWhere = "AND uf.sigla = '{$_SESSION['par']['estuf']}'";
                    }

                    if ($_SESSION['par']['adpid']){
                        $sql = "
                            SELECT  ent.pk_cod_entidade as codigo, 
                                    ent.pk_cod_entidade ||' - '|| ent.no_entidade as descricao
                            FROM ".SCHEMAEDUCACENSO.".tab_dado_escola esco

                            JOIN ".SCHEMAEDUCACENSO.".tab_entidade ent on ent.pk_cod_entidade = esco.fk_cod_entidade
                            JOIN ".SCHEMAEDUCACENSO.".tab_municipio mun on mun.pk_cod_municipio = ent.fk_cod_municipio
                            JOIN ".SCHEMAEDUCACENSO.".tab_estado uf on uf.pk_cod_estado = mun.fk_cod_estado
                            JOIN par.pfescolaeja pf on pf.pk_cod_entidade = esco.fk_cod_entidade
                            
                            WHERE pf.adpid = '{$_SESSION['par']['adpid']}'
                            $stWhere
                            AND ent.id_dependencia_adm = " . ($_SESSION['par']['itrid'] == 2 ? 3 : 2) . "
                            ORDER BY ent.no_entidade
                        ";
                        $nome = 'entid';
                        $$nome = $db->carregar($sql);
                    }
                    
                    $sql_combo = "
                        SELECT  ent.pk_cod_entidade as codigo, 
                                ent.pk_cod_entidade ||' - '|| ent.no_entidade as descricao
                        FROM ".SCHEMAEDUCACENSO.".tab_dado_escola esco

                        JOIN ".SCHEMAEDUCACENSO.".tab_entidade ent on ent.pk_cod_entidade = esco.fk_cod_entidade
                        JOIN ".SCHEMAEDUCACENSO.".tab_municipio mun on mun.pk_cod_municipio = ent.fk_cod_municipio
                        JOIN ".SCHEMAEDUCACENSO.".tab_estado uf on uf.pk_cod_estado = mun.fk_cod_estado

                        WHERE ent.id_dependencia_adm = ".($_SESSION['par']['itrid'] == 2 ? 3 : 2)."
                        $stWhere
                        ORDER BY ent.no_entidade
                    ";
                    combo_popup('entid', $sql_combo, 'Selecione a(s) Escola(s)', '360x460', '', '', '', $habilitado, true, false, '15', '800', null, true);
                ?>
            </td>
            <td align="center" valign="top" width="5%">
                <?
                    include_once APPRAIZ . "includes/workflow.php";

                    $dados_wf = array('adpid' => $_SESSION['par']['adpid']);
                    wf_desenhaBarraNavegacao( pgCriarDocumento($_SESSION['par']['adpid']), $dados_wf );
                ?>
            </td>
        </tr> 
        <tr>
            <td class="SubTituloCentro" colspan="2">
                <br>
                <? if ($habilitado == 'S') { ?>
                    <input type="button" name="btn_salvar" value="Salvar" onclick="salvar();">
                <? } ?>
            </td>
        <tr>
    </table>
</form>



	