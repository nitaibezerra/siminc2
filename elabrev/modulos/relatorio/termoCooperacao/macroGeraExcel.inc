<?php

if (!criaLoteMacroNCTeste()) {
    echo "<script>
			alert('Ocorreu um problema ao gerar o lote, tente novamente!');
			document.location = 'elabrev.php?modulo=relatorio/termoCooperacao/macroTC&acao=A';
		  </script>";
}

header('Content-type: application/xls');
header('Content-Disposition: attachment; filename="macro_termo_cooperacao_'.date('YmdHis').'.xls"');

// Recupera termos para geração das macros
$rs = recuperarLoteTermosTeste();

// Recupera quantidade maxima de previsão orçamentária do termo
$qtdMaxPrevisao = contaQtdMaxPrevisaoOrcamentariaTeste();

$arMesNum = array('/01/','/02/','/03/','/04/','/05/','/06/','/07/','/08/','/09/','/10/','/11/','/12/');
$arMesDsc = array('/jan/','/fev/','/mar/','/abr/','/mai/','/jun/','/jul/','/ago/','/set/','/out/','/nov/','/dez/');
$arRemoveCarcTexto = array('"',"'", "http://", "/", "(", ")");
?>

<?php if($rs): ?>

    <table>
        <thead>
        <tr>
            <th>UG PROPONENTE</th>
            <th>GESTÃO PROPONENTE</th>
            <th>RESPONSÁVEL 1 - PROPONENTE</th>
            <th>UG CONCEDENTE</th>
            <th>GESTAO CONCEDENTE</th>
            <th>RESPONSÁVEL 2 - CONCEDENTE</th>
            <th>NÚMERO ORIGINAL</th>
            <th>TIPO DE CADASTRO</th>
            <th>NÚMERO DO PROCESSO</th>
            <th>UG REPASSADORA</th>
            <th>GESTÃO REPASSADORA</th>
            <th>UG RECEBEDORA DO RECURSO</th>
            <th>GESTÃO RECEBEDORA</th>
            <th>TÍTULO</th>
            <th>INÍCIO DA VIGÊNCIA</th>
            <th>FIM DA VIGÊNCIA</th>
            <th>VALOR FIRMADO</th>
            <th>VALOR TOTAL</th>
            <th>DESCRIÇÃO</th>
            <th>JUSTIFICATIVA</th>
            <th>EXERCÍCIO 1</th>
            <th>EXERCÍCIO 2</th>
            <th>EXERCÍCIO 3</th>
            <th>EXERCÍCIO 4</th>
            <th>EXERCÍCIO 5</th>
            <th>EXERCÍCIO 6</th>
            <th>PRAZO 1</th>
            <th>PRAZO 2</th>
            <th>PRAZO 3</th>
            <th>PRAZO 4</th>
            <th>PRAZO 5</th>
            <th>% REAL</th>
            <?php if($qtdMaxPrevisao>0): ?>
                <?php for($x=1;$x<=$qtdMaxPrevisao;$x++): ?>
                    <th>EVENTO <?php echo $x; ?></th>
                    <th>ESFERA <?php echo $x; ?></th>
                    <th>PTRES <?php echo $x; ?></th>
                    <th>FONTE <?php echo $x; ?></th>
                    <th>PI <?php echo $x; ?></th>
                    <th>ND <?php echo $x; ?></th>
                    <th>VALOR ND <?php echo $x; ?></th>
                <?php endfor; ?>
            <?php endif; ?>
            <th>N° de Registro</th>
        </tr>
        </thead>
        <tbody>
        <?php foreach($rs as $k => $dado): ?>
            <tr>
                <td><?php echo $dado['ungcodproponente']; ?></td>
                <td><?php echo $dado['gescodproponente'] ? "'".$dado['gescodproponente']."" : ''; ?></td>
                <td><?php echo $dado['cpfreplegalproponente'] ? "'".$dado['cpfreplegalproponente']."" : ''; ?></td>
                <td><?php echo $dado['ungcodconcedente']; ?></td>
                <td><?php echo $dado['gescodconcedente'] ? "'".$dado['gescodconcedente']."" : ''; ?></td>
                <td><?php echo $dado['cpfreplegalconcedente'] ? "'".$dado['cpfreplegalconcedente']."" : ''; ?></td>
                <td><?php echo $dado['tcpid']; ?></td>
                <td>6</td>
                <td><?php echo $dado['tcpid']; ?></td>
                <td>152734</td>
                <td>'00001</td>
                <td><?php echo $dado['ungcodproponente']; ?></td>
                <td><?php echo "'".$dado['gescodproponente']; ?></td>
                <td><?php echo substr(str_replace($arRemoveCarcTexto, '', $dado['tcptitulo']), 0, 69); ?></td>
                <td><?php echo $dado['data_vigencia'] ? str_replace($arMesNum, $arMesDsc, formata_data($dado['data_vigencia'])) : ''; ?></td>
                <td><?php echo $dado['data_vigencia'] ? str_replace($arMesNum, $arMesDsc, makeDateSoma(formata_data($dado['data_vigencia']), 0, $dado['crdmesexecucao'])) : ''; ?></td>

                <?php
                // -- Ao chegar aqui, a execução sempre será do XLS
                $valorTotal = $dado['valor_total'] ? str_replace('.',',',$dado['valor_total']) : '';
                $valorTotalSIGEF = $dado['valor_total'] ? str_replace('.','',$dado['valor_total']) : '';
                ?>

                <td><?php echo $valorTotal; ?></td>
                <td><?php echo $valorTotalSIGEF; ?></td>
                <td><?php echo substr(str_replace($arRemoveCarcTexto, '', $dado['tcpobjetivoobjeto']), 0, 489); ?></td>
                <td><?php echo substr(str_replace($arRemoveCarcTexto, '', $dado['tcpjustificativa']), 0, 349); ?></td>
                <td></td>
                <td></td>
                <td></td>
                <td></td>
                <td></td>
                <td></td>
                <td>360</td>
                <td></td>
                <td></td>
                <td></td>
                <td></td>
                <td>10000</td>

                <?php

                /**
                 * Se vier [lotid] busca um lote existente
                 * se não, busca o ultimo lote que acaba de ser criado
                 */
                if (isset($_GET['lotid'])) {
                    $_lotid = (int) $_GET['lotid'];
                } else {
                    $_lotid = "(select lotid from elabrev.lotemacro order by lotid desc limit 1)";
                }

                $sql = "SELECT
                            *, substr(ndpcod, 1, 6) as natureza FROM monitora.previsaoorcamentaria pro
						LEFT JOIN monitora.ptres ptr ON ptr.ptrid = pro.ptrid
						LEFT JOIN monitora.pi_planointerno pi on pi.pliid = pro.pliid
						LEFT JOIN public.naturezadespesa ndp ON ndp.ndpid = pro.ndpid
						WHERE
						    pro.tcpid = {$dado['tcpid']} AND
							pro.proid IN (select lmi.proid from elabrev.lotemacroitens lmi where lmi.lotid = {$_lotid})
						ORDER BY pro.proid ASC";
                //ver($sql, d);
                $rsPrevisoes = $db->carregar($sql);
                ?>

                <?php if($qtdMaxPrevisao>0): ?>
                    <?php for($x=0;$x<$qtdMaxPrevisao;$x++): ?>

                        <?php if($rsPrevisoes[$x]['proid']>0): ?>
                            <td></td>
                            <td></td>
                            <td><?php echo $rsPrevisoes[$x]['ptres']; ?></td>
                            <td></td>
                            <td><?php echo $rsPrevisoes[$x]['plicod']; ?></td>
                            <td><?php echo $rsPrevisoes[$x]['natureza']; ?></td>
                            <td><?php echo str_replace('.','',$rsPrevisoes[$x]['provalor']); ?></td>
                        <?php else: ?>
                            <td>&nbsp;</td>
                            <td>&nbsp;</td>
                            <td>&nbsp;</td>
                            <td>&nbsp;</td>
                            <td>&nbsp;</td>
                            <td>&nbsp;</td>
                            <td>&nbsp;</td>
                        <?php endif; ?>

                    <?php endfor; ?>
                <?php endif; ?>
                <td><?php echo $k+1; ?></td>
            </tr>
        <?php endforeach; ?>
        </tbody>
    </table>

<?php endif; ?>