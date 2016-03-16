<?php

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    if ($_REQUEST['req'] == 'download') {
        if ($_REQUEST['arqid']) {
            $file = new FilesSimec("resenharecurso", $campos, "livro");
            $file->getDownloadArquivo($_REQUEST['arqid']);
        }
    }

    #Chamada de programa
    include APPRAIZ . "includes/cabecalho.inc";
    
    $db->cria_aba($abacod_tela, $url, $parametros);
    monta_titulo('Responsável Editora', 'Responsável Editora');

    $filtro[] = "edtstatus='A'";
    if (!$db->testa_superuser()) {
        $arrPerfil = arrayPerfil();
        if (in_array(PNLD_EDITORA, $arrPerfil)) {
            $sql = "SELECT edtid FROM livro.usuarioresponsabilidade WHERE usucpf='" . $_SESSION['usucpf'] . "' AND pflcod='" . PNLD_EDITORA . "' AND rpustatus='A'";
            $edtids = $db->carregarColuna($sql);
            if ($edtids) {
                $filtro[] = "edtid IN('" . implode("','", $edtids) . "')";
            } else {
                $filtro[] = "1=2";
            }
        }
    }

    $sql = "SELECT * FROM livro.editora WHERE " . implode(" AND ", $filtro) . " ORDER BY edtnome";
    $editoras = $db->carregar($sql);

    if ($editoras[0]){
?>
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

<?PHP
        $permitido = verificaDataAberturaFechamento('V');
        
        if( $permitido == 'S' ){
            foreach ($editoras as $editora){
?>
                <tr>
                    <td class='SubTituloCentro'><?= $editora['edtnome'] ?></td>
                </tr>
                <tr>
                    <td>
<?PHP
                $acao = "
                    <center>
                        <img src=\"../imagens/consultar.gif\" border=\"0\" title=\"Consultar\" style=\"cursor:pointer;\" onclick=\"window.location=\'livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid='||res.tpaid||'&colid='||col.colid||'&analise=true&tpaid=' || res.tpaid || '\';\">
                        <img src=\"../imagens/anexo.gif\" border=\"0\" title=\"Parecer\" style=\"cursor:pointer;\" onclick=\"window.location=\'livro.php?modulo=principal/pnld_resenha_colecao&acao=A&colid='||col.colid||'&req=download&arqid='||arq.arqid||'\';\">
                    </center>
                ";

                $res_acao = "
                    <center>
                        <img src=\"../imagens/consultar.gif\" border=\"0\" title=\"Consultar\" style=\"cursor:pointer;\" onclick=\"window.location=\'livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid='||res.tpaid||'&colid='||col.colid||'&analise=true&tpaid=' || res.tpaid || '\';\">
                        <img src=\"../imagens/anexo.gif\" border=\"0\" title=\"Parecer\" style=\"cursor:pointer;\" onclick=\"window.location=\'livro.php?modulo=principal/pnld_editora_analise&acao=A&colid='||col.colid||'&req=download&amp;arqid='||ra.arqid||'\';\">
                    </center>
                ";

                $sem_acao = " 
                    <center> 
                        <img src=\"../imagens/consultar_01.gif\" border=\"0\" title=\"Consultar\">
                        <img src=\"../imagens/anexo.gif\" border=\"0\" title=\"Parecer\" style=\"cursor:pointer;\" onclick=\"window.location=\'livro.php?modulo=principal/pnld_resenha_colecao&acao=A&colid='||col.colid||'&req=download&arqid='||arq.arqid||'\';\">
                    </center> 
                ";

                $sql = "
                    SELECT * FROM (
                        (SELECT DISTINCT
                        
                                CASE
                                    WHEN res.tpaid = 1 THEN '{$acao}'

                                    WHEN (res.tpaid = 2 and ra.tpaid = 7) THEN '$res_acao'
                                    WHEN (res.tpaid = 3 and ra.tpaid = 3) THEN '{$acao}'
                                    WHEN (res.tpaid = 3 and ra.tpaid = 6) THEN '{$acao}'
                                    WHEN (res.tpaid = 3 and ra.tpaid = 7) THEN '$res_acao'
                                    WHEN (res.tpaid = 3 AND ra.tpaid = 1) THEN '{$acao}'

                                    ELSE  '$sem_acao'
                                END as consulta,

                                com.comdsc,
                                col.colcodigo||' - '||col.coltitulo as colecao,

                                CASE
                                    WHEN (res.tpaid = 2 and ra.tpaid = 7) THEN '<span style=\"color:#cc0000;\">Resposta ao Parecer de Aprovação condicionado</span>'
                                    WHEN (res.tpaid = 3 and ra.tpaid = 3) THEN 'Parecer de Exclusão'
                                    WHEN (res.tpaid = 3 and ra.tpaid = 6) THEN 'Recurso da Editora'
                                    WHEN (res.tpaid = 3 and ra.tpaid = 7) THEN '<span style=\"color:#cc0000;\">Resposta ao Recurso</span>'
                                    WHEN (res.tpaid = 2 or  ra.tpaid = 2) THEN 'Parecer de Aprovação condicionado à correção de falhas pontuais'
                                    ELSE  tpa.tpadsc
                                END as recurso

                        FROM livro.colecao col

                        LEFT JOIN livro.resenharecurso res ON res.colid = col.colid
                        LEFT JOIN livro.resenhaavaliacao ra ON ra.colid = res.colid
                        LEFT JOIN livro.tipoparecer tpa ON tpa.tpaid = res.tpaid
                        INNER JOIN livro.editora edt ON edt.edtid = col.edtid AND edt.edtstatus = 'A'
                        INNER JOIN livro.componente com ON com.comid = col.comid
                        INNER JOIN public.arquivo arq ON arq.arqid = res.arqid
                        LEFT JOIN public.arquivo arqcond ON arqcond.arqid = res.arqidparecercondicionado

                        WHERE colstatus='A' AND ravstatus = 'A' AND rrestatus='A' AND edt.edtid='" . $editora['edtid'] . "' and col.prsano = '" . $_SESSION['exercicio'] . "'

                        ORDER BY com.comdsc, colecao, recurso )

                        UNION ALL

                        (SELECT DISTINCT CASE
                                    WHEN res.tpaid = 3 THEN
                                            '<center>
                                                    <img src=\"../imagens/consultar.gif\" border=\"0\" title=\"Consultar\" style=\"cursor:pointer;\" onclick=\"window.location=\'livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid='||res.tpaid||'&lvdid='||l.lvdid||'&analise=true\';\">
                                                    <img src=\"../imagens/anexo.gif\" border=\"0\" title=\"Parecer\" style=\"cursor:pointer;\" onclick=\"window.location=\'livro.php?modulo=principal/pnld_resenha_colecao&amp;acao=A&amp;lvdid='||l.lvdid||'&amp;req=download&amp;arqid='||arq.arqid||'\';\">
                                            </center>'
                                    WHEN res.tpaid = 2 THEN
                                            '<center>
                                                    <img src=\"../imagens/consultar.gif\" border=\"0\" title=\"Consultar\" style=\"cursor:pointer;\" onclick=\"window.location=\'livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid='||res.tpaid||'&lvdid='||l.lvdid||'&analise=true\';\">&nbsp;'
                                                    || case when arq.arqid is not null then '<img alt=\"Parecer\" title=\"Parecer\" src=../imagens/anexo.gif border=0 style=cursor:pointer; onclick=\"window.location=\'livro.php?modulo=principal/pnld_resenha_colecao&amp;acao=A&amp;lvdid='||l.lvdid||'&amp;req=download&amp;arqid='||arq.arqid||'\';\">' else '' end
                                                    || case when arqcond.arqid is not null then '<img alt=\"Parecer Condicionado\" title=\"Parecer Condicionado\" src=../imagens/anexo.gif border=0 style=cursor:pointer; onclick=\"window.location=\'livro.php?modulo=principal/pnld_resenha_colecao&amp;acao=A&amp;lvdid='||l.lvdid||'&amp;req=download&amp;arqid='||arqcond.arqid||'\';\">' else '' end
                                            || '</center>'

                                     ELSE '<center><img src=../imagens/anexo.gif border=0 style=cursor:pointer; onclick=\"window.location=\'livro.php?modulo=principal/pnld_resenha_colecao&amp;acao=A&amp;lvdid='||l.lvdid||'&amp;req=download&amp;arqid='||arq.arqid||'\';\"></center>'
                                END as consulta,

                                com.comdsc,
                                l.lvdcodigo||' - '||l.lvdtitulo as colecao,

                                CASE
                                    WHEN (res.tpaid = 2 and ra.tpaid = 7) THEN '<span style=\"color:#cc0000;\">Resposta ao Parecer de Aprovação condicionado</span>'
                                    WHEN (res.tpaid = 3 and ra.tpaid = 3) THEN 'Parecer de Exclusão'
                                    WHEN (res.tpaid = 3 and ra.tpaid = 6) THEN 'Recurso da Editora'
                                    WHEN (res.tpaid = 3 and ra.tpaid = 7) THEN '<span style=\"color:#cc0000;\">Resposta ao Recurso</span>'
                                    WHEN (res.tpaid = 2 or  ra.tpaid = 2) THEN 'Parecer de Aprovação condicionado à correção de falhas pontuais'
                                    ELSE  tpa.tpadsc
                                END as recurso

                        FROM livro.livrodidatico l
                        INNER JOIN livro.resenharecurso res ON l.lvdid = res.lvdid AND rrestatus='A'
                        LEFT JOIN livro.resenhaavaliacao ra ON ra.lvdid = res.lvdid AND ravstatus = 'A'
                        LEFT JOIN livro.tipoparecer tpa ON tpa.tpaid = res.tpaid or tpa.tpaid = ra.tpaid
                        LEFT JOIN seguranca.usuario usu ON usu.usucpf = res.rreusucpf
                        INNER JOIN livro.componente com ON com.comid = l.comid AND com.comid IN (5,7,18,19)
                        LEFT JOIN public.arquivo arq ON arq.arqid = res.arqid
                        LEFT JOIN public.arquivo arqcond ON arqcond.arqid = res.arqidparecercondicionado

                        WHERE edtid='" . $editora['edtid'] . "' and l.prsano = '" . $_SESSION['exercicio'] . "'

                        ORDER BY 1, colecao )
                    ) as foo
                ";
//ver($sql);
//echo "<pre>";
//echo $sql;
//echo "</pre>";
                $cabecalho = array("&nbsp;", "Área de conhecimento", "Coleção", "Tipo parecer");
                $alinhamento = Array('center', '', '', '');
                $tamanho = Array('4%', '20%', '30%', '40%');
                $db->monta_lista($sql, $cabecalho, 5000, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);
?>
                    </td>
                </tr>
<?PHP
            }
        echo '</table>';
            
        }else{
?>
<!--            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">-->
<!--                <tr>-->
<!--                    <td class="SubTituloEsquerda" colspan="7">-->
<!--                        <img border="0" align="absmiddle" width="18" src="../imagens/alerta_sistema.gif" style="margin-top:-4px;"/>-->
<!--                        <span style="font-size:14px;  margin:5px; margin-top:12px;">INFORMATIVO-->
<!--                    </td>-->
<!--                </tr>-->
<!--                <tr>-->
<!--                    <td width="2%">-->
<!--                        <br>-->
<!--                        <p style="font-size:24px;"> Srº(ª)s Representantes de Editoras. </p>-->
<!--                        <p style="font-size:24px;">-->
<!--                            A consulta aos Pareceres das coleções e obras inscritas no PNLD 2016 somente estará disponível após a publicação da portaria de resultados no DOU,-->
<!--                            com previsão de liberação no dia 29 de junho a partir das 20h.-->
<!--                        </p>-->
<!--                    </td>-->
<!--                </tr>-->
<!--            </table>-->
      <?PHP
        }
    }
?>