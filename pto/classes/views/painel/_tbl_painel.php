<table cellspacing="0" cellpadding="0" border="1" class="table table-striped table-bordered table-condensed tbl_painel">
    <tbody>
     <tr>
        <td class="alinharMeio"><b>Detalhamento do Projeto:</b></td>
        <td colspan="3"><?= $this->dado['solucao']['solobs'] ?></td>
    </tr>
    <tr>
<!--        rowspan="2"-->
        <td class="alinharMeio"><b>PNE</b></td>
        <td colspan="3"><b>Metas Vinculadas:</b> <?= $this->metaSolucao->getMetaPainel( $this->dado['idsExternos']['mpneid'] )  ?></td>
    </tr>
<!--    <tr>-->
<!--        <td colspan="3" style="color: red"><b>Estratégias vinculadas: </b> 12.1 / 12.7 / 12.8 / 12.13</td>-->
<!--    </tr>-->
    <tr>
        <td rowspan="2" class="alinharMeio"><b>Ações Vinculadas:</b></td>
        <td rowspan="2" colspan="1"><?= $this->acaoSolucao->getAcaoPainel($this->dado['idsExternos']['acsid']) ?></td>
        <td><b>Responsável SE:</b></td>
        <td><?= $this->responsavelSolucaoSe->getResponsavelPainel( $this->dado['idsExternos']['usucpf'], 'S' )  ?></td>
    </tr>
    <tr>
        <td><b>Responsável Secretaria/Autarquia:</b></td>
        <td><?= $this->responsavelSolucaoSeAut->getResponsavelPainel( $this->dado['idsExternos']['usucpf'], 'A' )  ?></td>
    </tr>
    </tbody>
</table>