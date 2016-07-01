<?php

verificaMuncod();

// Resultado
$rsProfissional = $obFolhaProfissionais->recuperarDadosProfissionalPorId($_REQUEST['fppid']);
?>
<table class="tabela" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
    <tr>
        <td colspan="2" class="subtitulocentro">Detalhe do profissional</td>
    </tr>
    <tr>
        <td class="SubtituloDireita" style="width: 19%">CPF</td>
        <td><?php echo $rsProfissional['fppcpf'] ?></td>
    </tr>
    <tr>
        <td class="SubtituloDireita">Nome</td>
        <td><?php echo $rsProfissional['fppnome'] ?></td>
    </tr>
    <tr>
        <td class="SubtituloDireita">Cargo</td>
        <td><?php echo $rsProfissional['cardsc'] ?></td>
    </tr>
    <tr>
        <td class="SubtituloDireita">Formação</td>
        <td><?php echo $rsProfissional['fordsc'] ?></td>
    </tr>
    <tr>
        <td class="SubtituloDireita">Vínculo</td>
        <td><?php echo $rsProfissional['vicdsc'] ?></td>
    </tr>
    <tr>
        <td class="SubtituloDireita">Situação</td>
        <td><?php echo $rsProfissional['sitdsc'] ?></td>
    </tr>
    <tr>
        <td class="SubtituloDireita">Carga horária</td>
        <td><?php echo $rsProfissional['fppcargahoraria'] ?></td>
    </tr>
    <tr>
        <td class="SubtituloDireita">Tipo de Lotação</td>
        <td><?php echo $rsProfissional['lotdsc'] ?></td>
    </tr>
    <tr>
        <td class="SubtituloDireita">Descrição da Lotação</td>
        <td><?php echo empty($rsProfissional['entcodent']) ? $rsProfissional['fppdesclotacao'] : $rsProfissional['entnome'] ?></td>
    </tr>
    <tr>
        <td class="SubtituloDireita">Etapa</td>
        <td><?php echo $rsProfissional['etpdsc'] ?></td>
    </tr>
    <tr>
        <td class="SubtituloDireita">Zona</td>
        <td><?php echo $rsProfissional['fppzona'] == 'ZR' ? 'Zona rural' : 'Zona urbana' ?></td>
    </tr>
    <tr>
        <td class="SubtituloDireita">Segmento da Educação Básica</td>
        <td><?php echo $rsProfissional['sebdsc'] ?></td>
    </tr>
    <tr>
        <td class="SubtituloDireita">Salário Base(R$)</td>
        <td><?php echo number_format($rsProfissional['fppsalbase'], 2, ',', '.') ?></td>
    </tr>
    <tr>
        <td class="SubtituloDireita">Adicionais e Gratificações(R$)</td>
        <td><?php echo number_format($rsProfissional['fppadigrat'], 2, ',', '.') ?></td>
    </tr>
    <tr>
        <td class="SubtituloDireita">Remuneração(Sal. Base + Adic.)</td>
        <td><?php echo number_format($rsProfissional['fppsalbase']+$rsProfissional['fppadigrat'], 2, ',', '.') ?></td>
    </tr>
    <tr bgcolor="#cccccc">
       <td></td>
       <td>
        <input type="button" value="Voltar" class="voltar" id="lista" />
       </td>
    </tr>
</table>