<?php


$capturaTipo = cte_pegarItrid($_SESSION['inuid']) == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ? "E" : "M";
$aciid       = (integer) $_REQUEST['aciid'];

$sql         = '
select
    p.crtid
from
    cte.acaoindicador a
inner join
    cte.pontuacao p on p.ptoid = a.ptoid
where
    a.aciid = ' . $aciid;

$crtid = (integer) $db->pegaUm( $sql );

if ($capturaTipo == 'M') {
    $sql = '
    select
        sam.muncod
    from
        cte.instrumentounidade iu
    inner join
        cte.subacaomunicipio as sam on sam.muncod = iu.muncod
    where
        inuid = ' . $_SESSION['inuid'];

    $muncod        = $db->pegaUm($sql);
    $joinmunicipio = '';
    $andmunicipio  = '';

    if ($muncod) {
        $joinmunicipio = ' left join cte.subacaomunicipio sam on sam.ppsid = psub.ppsid and sam.muncod = \'' . $muncod . '\'';
        $andmunicipio  = ' and psub.ppsid not in (select psa.ppsid from cte.proposicaosubacao psa 
                                					INNER JOIN cte.subacaomunicipio sam on sam.ppsid = psa.ppsid
                                				where sam.ppsid not in (select ppsid from cte.subacaomunicipio where  muncod =\'' . $muncod . '\'))';
    } else {
    	# Busca os que não forem municipios ( Estados )
        $andmunicipio  = ' and psub.ppsid not in (select psa.ppsid from cte.proposicaosubacao psa INNER JOIN cte.subacaomunicipio sam on sam.ppsid = psa.ppsid)';
    }

    $sql =	'
    select
        psub.*,
        prg.prgid,
        prg.prgdsc
    from
        cte.proposicaosubacao psub
    left outer
        join cte.proposicaoacao pro on pro.ppaid = psub.ppaid
    left outer
        join cte.criterio crt on crt.crtid=pro.crtid
    inner join
        cte.programa prg on prg.prgid = psub.prgid
    ' . $joinmunicipio . '
    where
        pro.crtid = ' . $crtid . '
        ' . $andmunicipio . '
        and psub.ppsid not in(select coalesce( ppsid, 0 ) from cte.subacaoindicador where aciid = ' . $aciid . ' )
    order by
        ppsordem';
} else {
    $sql   = '
    select
        *, prg.prgdsc
    from
        cte.proposicaosubacao psub
     left join
        cte.programa prg on prg.prgid = psub.prgid
    where
        indid = (SELECT indid FROM cte.criterio WHERE crtid = ' . $crtid . ')';
}

$dadosProposta = $db->carregar($sql);
?>
<html>
<head>
<title>Propostas de Subação</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel="stylesheet" type="text/css" href="../includes/listagem.css" />
<script type="text/javascript">
    function enviarProposta(ppsid, ppsdsc, undid, ppstexto, ppsobjetivo, prgid, ppsordem, ppsmetodologia, frmid, programa, indqtdporescola)
    {
        var attrs                = [];
        attrs['ppsid']           = ppsid;
        attrs['ppsdsc']          = ppsdsc;
        attrs['undid']           = undid;
        attrs['ppstexto']        = ppstexto;
        attrs['ppsobjetivo']     = ppsobjetivo;
        attrs['prgid']           = prgid;
        attrs['ppsordem']        = ppsordem;
        attrs['ppsmetodologia']  = ppsmetodologia;
        attrs['frmid']           = frmid;
        attrs['indqtdporescola'] = indqtdporescola;

        window.opener.propostaSubAcao(attrs);
        window.close();
    }
    
</script>
</head>
<body leftmargin="0" topmargin="0" marginwidth="0" marginheight="0" bgcolor="#ffffff" style="background-image: url( '/imagens/fundo.gif' ); background-repeat: repeat-y;">
<table width="100%" align="center" border="0" class="tabela" cellpadding="1" cellspacing="1" >
    <tbody>
        <tr>
           <td style="padding:1px; background-color:#fafafa; color:#404040; vertical-align: top;" >
                <form method="POST" name="formulario2">
                    
                    <table width="100%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                        <tr>
                            <td class="SubTituloEsquerda"><strong>Selecione:</strong></td>
                            <td class="SubTituloEsquerda"><strong>Descrição Proposta de Sub Ação:</strong></td>
                        </tr>

<?php
                        if (!empty($dadosProposta)) {
                            for ($i = 0; $i < count($dadosProposta);$i++ ) {
                                $dadosForma   = $db->carregar(" select fe.frmid,fe.frmdsc from cte.formaexecucao fe
                                                              where fe.frmid='".$dadosProposta[$i]['frmid']."' ");	
                                $dadosUnidade = $db->carregar(" select um.undid,um.unddsc from cte.unidademedida um 
                                                              where um.undid='".$dadosProposta[$i]['undid']."' ");

                                $ppsid           = addslashes($dadosProposta[$i]['ppsid']);
                                $ppsdsc          = addslashes($dadosProposta[$i]['ppsdsc']);
                                $undid           = addslashes($dadosProposta[$i]['undid']);
                                $ppstexto        = addslashes($dadosProposta[$i]['ppstexto']);
                                $ppsobjetivo     = addslashes($dadosProposta[$i]['ppsobjetivo']);
                                $prgid           = addslashes($dadosProposta[$i]['prgid']);
                                $ppsordem        = addslashes($dadosProposta[$i]['ppsordem']);
                                $ppsmetodologia  = addslashes($dadosProposta[$i]['ppsmetodologia']);
                                $frmid           = addslashes($dadosProposta[$i]['frmid']);
                                $indqtdporescola = addslashes($dadosProposta[$i]['indqtdporescola']);
                                $subForma        = addslashes($dadosForma[0]['frmdsc']);
                                $subUnidade      = addslashes($dadosUnidade[0]['unddsc']);
                                $programa        = addslashes($dadosProposta[$i]['prgdsc']);

?>

                                <tr>
                                    <td class="TextoEsquerda">
                                      <input type="radio" name="opcao" value="<?= $i; ?>" onclick="enviarProposta('<?= $ppsid ?>','<?= $ppsdsc ?>','<?= $undid ?>','<?= $ppstexto ?>','<?= $ppsobjetivo ?>','<?= $prgid ?>','<?= $ppsordem ?>','<?= $ppsmetodologia ?>','<?= $frmid ?>','<?= $programa ?>','<?php echo $indqtdporescola; ?>');"
                                        />
                                    </td>
                                    <td class="TextoEsquerda">
                                        <strong>Nome:</strong> <?= $ppsdsc; ?><br>
                                        <strong>Metodologia:</strong> <?=$ppsmetodologia; ?><br>
                                        <strong>Programa:</strong> <?= $programa; ?><br>
                                        <strong>Forma:</strong> <?=  $subForma; ?><br>
                                        <strong>Unidade:</strong> <?= $subUnidade; ?><br>
                                    </td>
                                </tr>
<?php

                            } // if (!empty($dadosProposta))
                        } // for ($i = 0; $i < count($dadosProposta);$i++ )

?>
                    </table>
                </form>	
            </td>
         </tr>
    </tbody>
</table>
</body>
</html>