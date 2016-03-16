<html>
    <head>
        <script type="text/javascript" src="../includes/funcoes.js"></script>
        <script type="text/javascript" src="../includes/prototype.js"></script>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    </head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">

<?php

ini_set("memory_limit", "1024M");

include APPRAIZ. 'includes/classes/relatorio.class.inc';
$sql   = monta_sql();
$agrup = monta_agp();
$dados = $db->carregar($sql);
$col = montaColunas($_REQUEST['agrupador'], $_REQUEST['chkExibeFoto']);
$r = new montaRelatorio();
$r->setColuna($col);
$r->setAgrupador($agrup, $dados);
$r->setTotNivel(false);
$r->setBrasao(true);
$r->setMonstrarTolizadorNivel(false);
$r->setTolizadorLinha(false);
$r->setTotalizador(false);
$r->setTextoTotalizador();
$r->setEspandir($_REQUEST['expandir']);
echo $r->getRelatorio();

function monta_sql(){

    $ordenacao = montaOrdenacao($_REQUEST['ordenacao']);

    $condicao = montaCondicao($_REQUEST);


    $sql = "
       select v.vstid
      ,COALESCE(CAST(v.vstnumcracha as varchar(100)) ,v.vsnumcrachasistema) as vstnumcracha
      ,to_char(v.vstdatentrada, 'DD/MM/YYYY HH24:MI') as vstdatentrada
      ,to_char(v.vstdatsaida, 'DD/MM/YYYY HH24:MI') as vstdatsaida
      ,edificio.edfdsc || ' - ' || andar.edadsc || ' - ' || d.dstsigla dstdsc
      ,case when v.vttid is not null
            then case when td1.tiddsc = 'CPF' and length(vt1.vttdoc) = 11
                      then substr(vt1.vttdoc,1,3) || '.' || substr(vt1.vttdoc,4,3) || '.' || substr(vt1.vttdoc,7,3) || '-' || substr(vt1.vttdoc,10,2)
                      else vt1.vttdoc
                 end
            else case when a.vttid is not null
                      then case when td2.tiddsc = 'CPF' and length(vt2.vttdoc) = 11
                                then substr(vt2.vttdoc,1,3) || '.' || substr(vt2.vttdoc,4,3) || '.' || substr(vt2.vttdoc,7,3) || '-' || substr(vt2.vttdoc,10,2)
                                else vt2.vttdoc
                           end
                      else substr(s2.nu_cpf,1,3) || '.' || substr(s2.nu_cpf,4,3) || '.' || substr(s2.nu_cpf,7,3) || '-' || substr(s2.nu_cpf,10,2)
                 end
       end as vttdoc
      ,case when v.vttid is not null
            then vt1.vttnome
            else case when a.vttid is not null
                      then vt2.vttnome
                      else s2.no_servidor
                 end
       end as vttnome
      ,case when v.vttid is not null
            then case when length(vt1.vtttel) = 10
                      then '(' || substr(vt1.vtttel,1,2) || ')' || substr(vt1.vtttel,3,4) || '-' || substr(vt1.vtttel,7,4)
                      else case when length(vt1.vtttel) = 8
                                then substr(vt1.vtttel,1,4) || '-' || substr(vt1.vtttel,5,4)
                                else vt1.vtttel
                           end
                 end
            else case when a.vttid is not null
                      then case when length(vt2.vtttel) = 10
                                then '(' || substr(vt2.vtttel,1,2) || ')' || substr(vt2.vtttel,3,4) || '-' || substr(vt2.vtttel,7,4)
                                else case when length(vt2.vtttel) = 8
                                          then substr(vt2.vtttel,1,4) || '-' || substr(vt2.vtttel,5,4)
                                          else vt2.vtttel
                                     end
                           end
                      else null
                 end
       end as vtttel
      ,case when v.vttid is not null
            then td1.tiddsc
            else case when a.vttid is not null
                      then td2.tiddsc
                      else 'CPF'
                 end
       end as tiddsc
      ,
		s3.sg_unidade_org as lotacao,
		usu.usunome as usunome,
      '<img border=\"0\" width=\"45\" height=\"45\" src=\"?modulo=principal/imagem&acao=A&idArquivo=' || case when foto1.arqid is not null
                                                                                                              then foto1.arqid
                                                                                                              else case when foto2.arqid is not null
                                                                                                                        then foto2.arqid
                                                                                                                        else foto3.arqid
                                                                                                                   end
                                                                                                         end || '\">' as img
  from sca.visita v
 inner join sca.destino d
    on d.dstid = v.dstid
 inner join sca.edificioandar andar
     on andar.edaid=v.edaid
 inner join sca.edificio edificio
     on edificio.edfid=andar.edfid
  left join sca.visitante vt1
    on v.vttid = vt1.vttid
  left join sca.tipodocumento td1
    on td1.tidid = vt1.tidid
  left join sca.autorizado a
 inner join sca.autorizacao t
    on a.autid = t.autid
  left join sca.vwservidorativo s1
    on t.nu_matricula_siape = s1.nu_matricula_siape
    on v.audid = a.audid
  left join sca.visitante vt2
    on a.vttid = vt2.vttid
  left join sca.tipodocumento td2
    on td2.tidid = vt2.tidid
  left join sca.vwservidorativo s2
    on a.nu_matricula_siape = s2.nu_matricula_siape
  left join sca.visitantefoto foto1
    on foto1.vttid = vt1.vttid
  left join sca.visitantefoto foto2
    on foto2.vttid = vt2.vttid
  left join sca.visitantefoto foto3
    on foto3.nu_matricula_siape = s2.nu_matricula_siape
  left join seguranca.usuario usu on v.usucpfentrada = usu.usucpf
  left join sca.vwservidorativo s3
    on v.nu_matricula_siape = s3.nu_matricula_siape
   ".$condicao." ".$ordenacao." ";

    return $sql;
}

function monta_agp(){

    $agp = array(
        "agrupador" => array(
            array(
                "campo" => "vstid",
                "label" => "Nº da Visita ",
                     "type" => "string"
            )
        ),
        "agrupadoColuna" => array(
            'vttdoc',
            'vttnome',
            'vstdatentrada',
            'vstdatsaida',
            'dstdsc',
            'lotacao',
            'usunome',
            'img')
    );

    return $agp;

}

function montaColunas($campos, $exibeFoto){
    $colunasSelecionadas = array();

    foreach ($campos  as $campo){
        $nomeCampo = explode(".", $campo);
        $coluna['campo'] = $nomeCampo[1];
        switch ($nomeCampo[1]) {
            case "vttdoc":
                 $coluna['label'] = "Nº do Documento";
                 break;
             case "vttnome":
                 $coluna['label'] = "Nome";
                 break;
             case "vstdatentrada":
                 $coluna['label'] = "Entrada";
                 break;
             case "vstdatsaida":
                 $coluna['label'] = "Saída";
                 break;
             case "dstdsc":
                 $coluna['label'] = "Destino";
                 break;
             case "lotacao":
                 $coluna['label'] = "Lotação";
                 break;
             case "usunome":
                 $coluna['label'] = "Quem Autorizou";
                 break;
        }
        $coluna["type"] = "string";

        $colunasSelecionadas[] = $coluna;
    }

    if($exibeFoto=='on'){
        $colunasSelecionadas[] = array("campo" => "img","label" => "Foto");
    }

    return $colunasSelecionadas;

}

function montaOrdenacao($ordem){
    $ordenacao = " ORDER BY ";
    switch ($ordem) {
        case "DHE":
            $ordenacao .="v.vstdatentrada";
            break;
        case "DHS":
            $ordenacao .="v.vstdatsaida";
            break;
       case "NAU":
            $ordenacao .="vt1.vttnome";
            break;
       case "LOT":
            $ordenacao .="d.dstdsc";
            break;
       default:
            $ordenacao .="v.vstdatentrada";
    }
    return $ordenacao;
}

function montaCondicao($request){

    extract($request);
    $condicao = " WHERE 1=1 ";

    $dtInicio = addslashes($dtInicio);
    $dtFinal = addslashes($dtFinal);

    $hrInicial = (!empty($hrInicial))? addslashes($hrInicial).":00":"00:00:00";
    $hrFinal = (!empty($hrFinal))? addslashes($hrFinal).":00":"00:00:00";

    $nomeVisitante = addslashes($nomeVisitante);
    $destino = addslashes($destino);
    $chkVisitanteSemRegistroSaida = addslashes($chkVisitanteSemRegistroSaida);
    $chkPermanenciaSuperior = addslashes($chkPermanenciaSuperior);
    $chkVisitanteSemDocumento = addslashes($chkVisitanteSemDocumento);

    $dtFinal = formata_data_sql($dtFinal);
    $dtInicio = formata_data_sql($dtInicio);

    $dthrInicio = (!empty($dtInicio))? $dtInicio ." ". $hrInicial:null;
    $dthrFinal = (!empty($dtFinal))? $dtFinal ." ". $hrFinal:null;

    if($dthrInicio !=null && !empty($dthrInicio)){
        $condicao .= " AND v.vstdatentrada>='" . $dthrInicio . "' ";
    }
    if($dthrFinal !=null && !empty($dthrFinal)){
        $condicao .= " AND v.vstdatentrada<='" . $dthrFinal . "' ";
    }
    if($nomeVisitante!=null && !empty($nomeVisitante)){
        $condicao .= " AND (vt1.vttnome ilike '%".$nomeVisitante."%' or vt2.vttnome ilike '%".$nomeVisitante."%' or s2.no_servidor ilike '%".$nomeVisitante."%') ";
    }
    if($destino!=null && !empty($destino)){
        $condicao .= " AND edificio.edfdsc || ' - ' || andar.edadsc || ' - ' || d.dstsigla ilike '%".$destino."%' ";
    }
    if($chkVisitanteSemRegistroSaida!=null && $chkVisitanteSemRegistroSaida=="on"){
        $condicao .= " AND v.vstdatsaida is null ";
    }
    if($chkPermanenciaSuperior!=null && $chkPermanenciaSuperior=="on"){
        $condicao .= " AND date(v.vstdatsaida) - date(v.vstdatentrada) > 1";
    }
    if($chkVisitanteSemDocumento!=null && $chkVisitanteSemDocumento=="on"){
        $condicao .= " AND (vt1.vttid is not null and vt1.tidid is null or vt2.vttid is not null and vt2.tidid is null) ";
    }

    return $condicao;
}

?>
<script>
    Event.observe(window, 'load', function() {
        var trs = $$('table.tabela tr');
        for(var i = 3; i < trs.length; i++){
            trs[i].cells[0].style.display = 'none';;
        }
      });
</script>
</body>
</html>