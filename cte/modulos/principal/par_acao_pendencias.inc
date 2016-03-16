<?php
//
// $Id$
//

cte_verificaSessao();

$habilita         = cte_podeElaborarPlanoDeAcoes($_SESSION['inuid']);
$estado_documento = wf_pegarEstadoAtual( cte_pegarDocid( $_SESSION['inuid'] ) );

function tratarData( $texto )
{
    $texto = trim( $texto );
    $dados = explode( "/", $texto );
    $dia = (integer) $dados[0];
    $mes = (integer) $dados[1];
    $ano = (integer) $dados[2];
    if ( !checkdate( $mes, $dia, $ano ) )
    {
        return "";
    }

    return sprintf("%02d/%02d/%04d", $dia, $mes, $ano );
}

$_REQUEST['acidtinicial'] = tratarData( $_REQUEST['acidtinicial'] );
$_REQUEST['acidtfinal']   = tratarData( $_REQUEST['acidtfinal'] );

$inicio = $_REQUEST['acidtinicial'] ? "'" . $_REQUEST['acidtinicial'] . "'" : "null";
$final  = $_REQUEST['acidtfinal']   ? "'" . $_REQUEST['acidtfinal']   . "'" : "null";

if ( cte_pegarItrid( $_SESSION["inuid"] ) == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL )
{
	$_acilocalizador = "M";
}
else
{
	if ( isset( $_REQUEST["aciid"] ) )
	{
		$sql = "select acilocalizador from cte.acaoindicador where aciid = " . ( (integer) $_REQUEST["aciid"] );
		$_acilocalizador = $db->pegaUm( $sql );
	}
	else
	{
		$_acilocalizador = $_REQUEST["acilocalizador"];
	}
}

if ($habilita && trim($_REQUEST['action']) != '') {
    if ($_REQUEST['action'] == "edit") {
        if (strlen($_REQUEST['aciresultado']) > 500) {
            echo '<script type="text/javascript">alert("O número máximo de caractéres para o campo Resultado é 500!");history.back();</script>';
        } else {
        	if($_REQUEST['acidescricao'] && $_REQUEST['aciresponsavel'] && $_REQUEST['acicargo'] && $_REQUEST['aciresultado'] ){
            $sql = "update cte.acaoindicador set
                        acidsc          = '".$_REQUEST['acidescricao'] . "',
                        acirpns         = '".$_REQUEST['aciresponsavel']."',
                        acicrg          = '".$_REQUEST['acicargo']."',
                        acidtinicial    = " . $inicio .",
                        acidtfinal      = " . $final .",
                        acirstd         = '".$_REQUEST['aciresultado']."',
                        acidata         = '". date("Y-m-d H:i:s") ."',
                        usucpf          = '". $_SESSION['usucpf'] ."',
                        aciparecer      = '". $_REQUEST['aciparecer'] ."',
                        acilocalizador  = '".$_acilocalizador."'
                    where
                        aciid='".$_REQUEST['aciid']."'";

            $db->executar($sql);
            $db->commit();
              echo '<script type="text/javascript">'
	            .'window.opener.location.reload();'
	            .'alert("Ação alterada com sucesso.");'
	            .'window.opener.focus();'
	            .'window.close();'
	            .'</script>';
           // $db->sucesso('principal/par_acao', '&aciid=' . $_REQUEST['aciid']);
        	}else{
        		echo '<script type="text/javascript">'
	            .'alert("Ocorreu um problema ao tentar gravar os dados.");'
	            .'window.close();'
	            .'</script>';
        	}
        }

        exit;
    }
}
$_REQUEST['acilocalizador'] = $_acilocalizador;
unset( $_acilocalizador );


if ($_REQUEST['acilocalizador'] == "E"){
    $tipo_modulo    = " Rede Estadual";
    $acilocalizador = $_REQUEST['acilocalizador'];
} elseif ($_REQUEST['acilocalizador'] == "M") {
    $tipo_modulo    = " Rede Municipal";
    $acilocalizador = $_REQUEST['acilocalizador'];
}


if($habilita == "true"){
    $habil='S';
} else {
    $habil='N';
}

if (trim($_REQUEST['ptoid']) !== ""){
    $varptoid = $_REQUEST['ptoid'];
} elseif(trim($_REQUEST['indid']) !== "") {
    $varptoid = $db->pegaUm(" select ptoid from cte.pontuacao where ptostatus = 'A' and inuid = ".$_SESSION['inuid']." and indid=".$_REQUEST['indid']."");
} elseif ($_REQUEST['aciid']) {
    $varptoid = $db->pegaUm(" select ptoid from cte.acaoindicador where aciid = '".$_REQUEST['aciid']."'");
} else {
    // nao faz nada
}

$acaoIndicador = array('aciid'          => '',
                       'acidsc'         => '',
                       'acirpns'        => '',
                       'acicrg'         => '',
                       'acidtinicial'   => '',
                       'acidtfinal'     => '',
                       'acirstd'        => '',
                       'acilocalizador' => $acilocalizador,
                       'ptoid'          => '',
                       'aciparecer'     => '');

$sql = "select
            aci.aciparecer,
            aci.aciid,
            aci.ptoid,
            aci.acidsc,
            aci.acirpns,
            aci.acicrg,
            aci.acidtinicial,
            aci.acidtfinal,
            aci.acirstd,
            trim(aci.acilocalizador)
        from
            cte.acaoindicador aci
        where
            --aci.ptoid='" .  $varptoid . "'
           aci.aciid = '" . ( (integer) $_REQUEST["aciid"] ) . "'
        order by
            aci.aciid desc";

$acaoIndicador  = array_merge($acaoIndicador, (array) $db->recuperar($sql));

//dump($acaoIndicador, true);

$aciId          = $acaoIndicador['aciid'];
$acidescricao   = $acaoIndicador['acidsc'];
$aciresponsavel = $acaoIndicador['acirpns'];
$acicargo       = $acaoIndicador['acicrg'];
$acidtinicial   = $acaoIndicador['acidtinicial'];
$acidtfinal     = $acaoIndicador['acidtfinal'];
$aciresultado   = $acaoIndicador['acirstd'];
$acilocalizador = $acaoIndicador['acilocalizador'];
$aciparecer     = $acaoIndicador['aciparecer'];

if ($acilocalizador == "M") {
    $tipo_modulo = " Rede Municipal";
} else {
    $tipo_modulo = " Rede Estadual";
}

$sql = "select
            dim.dimid,
            dim.dimcod,
            dim.dimdsc,
            are.ardid,
            are.ardcod,
            are.arddsc,
            ind.indid,
            ind.indcod,
            ind.inddsc
        from
            cte.dimensao dim
        left outer join
            cte.areadimensao are on are.dimid=dim.dimid 
        left outer join
            cte.indicador ind on ind.ardid=are.ardid
        left outer join
            cte.pontuacao pto on pto.indid=ind.indid and ptostatus = 'A'
        where pto.ptoid='" . $varptoid . "'";

$hieraquia = array('dimid'  => '',
                   'dimcod' => '',
                   'dimdsc' => '',
                   'ardid'  => '',
                   'ardcod' => '',
                   'arddsc' => '',
                   'indid'  => '',
                   'indcod' => '',
                   'inddsc' => '');

$hieraquia = array_merge($hieraquia, (array) $db->recuperar($sql));


$dimensaoId            = $hieraquia['dimid'];
$dimensaoCod           = $hieraquia['dimcod'];
$dimensaoDescricao     = $hieraquia['dimdsc'];
$areaDimensaoId        = $hieraquia['ardid'];
$areaDimensaoCod       = $hieraquia['ardcod'];
$areaDimensaoDescricao = $hieraquia['arddsc'];
$indicadorId           = $hieraquia['indid'];
$indicadorCod          = $hieraquia['indcod'];
$indicadorDescricao    = $hieraquia['inddsc'];


//controle do tipo de acao 
if ($aciId != "") {
    //$tipoPar  		 =  $db->carregar(" select tp.parid, tp.partipo,tp.pardsc from cte.tipopar tp where tp.parid='".$acaoIndicador[0]['parid']."' ");
    //$vartipopar      =  $tipoPar[0]['partipo'];
    $acao = "edit";
} else {
    $vartipopar      = "0";
    $acao = "add";
}

if (cte_pegarMuncod($_SESSION['inuid']) > 0) {
    $qtdProposta     = "0";
    $mostraLista     = "false";
    $propostaacaodsc = "";

    $dadosProposta   =  $db->carregar(" select pro.ppaid,pro.crtid,pro.ppadsc from cte.proposicaoacao pro 	
    left outer join cte.criterio crt on crt.crtid=pro.crtid 
    left outer join cte.pontuacao pot on pot.crtid=crt.crtid
    where pot.ptoid=". ( (integer) $varptoid ) );

    $qtdProposta     = count($dadosProposta); 
    //echo "quantidade: ".$qtdProposta ;
    $propostaacaodsc = $dadosProposta[0]['ppadsc'];
    $criterioId      = $dadosProposta[0]['crtid'];

    if( $qtdProposta > 1) {
        $mostraLista     = "true";	
    }else if($qtdProposta == 1){
        $mostraLista     = "false";
        if($_REQUEST['aciid']==""){
        $acidescricao    = $propostaacaodsc;
        }
    }

    $habilProposta = $habil;
} else {
    $habilProposta = "S";
}

$db->cria_aba( $abacod_tela, $url, '&indid='. $indicadorId);
cte_montaTitulo( $titulo_modulo );
?>
<html>
<head>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css">
<link rel="stylesheet" type="text/css" href="/includes/listagem.css">
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
</head>
<body>


<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
    
    <tbody>
        <tr>
            <td style="padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;" colspan="4">
                <form method="POST" name="formulario">
                    <input type="hidden" name="formulario" value="1"/>
                    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                        <colgroup>
                            <col style="width: 25%;"/>
                            <col style="width: 75%;"/>
                        </colgroup>
                        <tbody>
                            
                        <tr>
                            <td class="SubTituloDireita">Dimensão:</td>
                            <td><?= $dimensaoCod ?>. <?=$dimensaoDescricao; ?></td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Área:</td>
                            <td><?= $areaDimensaoCod ?>. <?=$areaDimensaoDescricao; ?></td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Indicador:</td>
                            <td><?= $indicadorCod ?>. <?=$indicadorDescricao; ?></td>
                        </tr>
                        
                        <tr>
                            <td class="SubTituloDireita">Demanda:</td>
                            <td> <?=$tipo_modulo; ?></td>
                        </tr>
                            
                            <tr>
                                <td align='right' class="SubTituloDireita">Descrição da Ação:</td>
                                <td>
                                  <?  if($mostraLista=="true"){ ?>
                                 <!--    se tiver uma proposta de ação não e nescessário -->
                                    <div align="left" >
                                        <a href="javascript:janelaSecundaria('cte.php?modulo=principal/proposta/proposta_acao&acao=A&ctrid=<?=$criterioId ?>&ptoid=<?= $_REQUEST['ptoid'] ?>&acilocalizador=<?= $_REQUEST['acilocalizador'] ?>')"> 
                                        Clique neste link para abrir opções de proposta de ação
                                        </a> 
                                    </div><br>
                                  <?  } ?>
                                    <? $sufixo = $habilProposta == "S" ? "" : "_fake" ; ?>
                                    <? ${'acidescricao' . $sufixo} = $acidescricao; ?>
                                    <?= campo_textarea('acidescricao' . $sufixo, 'S', $habilProposta, '', 70, 3, 500 ); ?>
                                    <?php if ( $habilProposta == "N" ) : ?>
                                        <input
                                            type="hidden"
                                            name="acidescricao"
                                            id="acidescricao"
                                            value="<?= simec_htmlentities( $acidescricao ) ?>"
                                        />
                                    <?php endif; ?>
                                </td>
                            </tr>
                            <tr>
                                <td align='right' class="SubTituloDireita">Nome do Responsável:</td>
                                <td><?=campo_texto( 'aciresponsavel', 'S', $habil, '', 50, 45, '', '' );?></td>
                            </tr>
                            <tr>
                                <td align='right' class="SubTituloDireita">Cargo do Responsável:</td>
                                <td><?=campo_texto( 'acicargo', 'S', $habil, '', 50, 45, '', '' );?></td>
                            </tr>
                            <tr>
                                <td align='right' class="SubTituloDireita">Período Inicial:</td>
                                <td><?= campo_data( 'acidtinicial', 'S', $habil, '', 'S' ); ?></td>
                            </tr>
                            <tr>
                                <td align='right' class="SubTituloDireita">Período Final:</td>
                                <td><?= campo_data( 'acidtfinal', 'S', $habil, '', 'S' ); ?></td>
                            </tr>
                            <tr>
                                <td align='right' class="SubTituloDireita">Resultado Esperado:</td>
                                <td><?= campo_textarea( 'aciresultado', 'S', $habil, '', 70, 3, 500 ); ?></td>
                            </tr>

                            
                            <?php if ( cte_podeEditarParecer( $_SESSION['inuid'] ) || !empty( $aciparecer ) ): ?>
                                <tr>
                                    <td align='right' class="SubTituloDireita">Parecer:</td>
                                    <td>
                                    <?php
                                        $habil_parecer = cte_podeEditarParecer( $_SESSION['inuid'] ) ? 'S' : 'N'; 
                                        campo_textarea( 'aciparecer', 'S', $habil_parecer, '', 70, 3, 0 );
                                    ?>
                                    </td>
                                </tr>
                            <?php endif; ?>
                            
                            <tr style="background-color: #cccccc">
                                <td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
                                <td>
                                    <?if($acao=="edit"){ ?>
                                    <input type="hidden" name="aciid" value="<?=$aciId ?>" />
                                    <? } ?>
                                    <input type="hidden" name="partipo"         value="<?=$vartipopar ?>" />
                                    <input type="hidden" name="ptoid"           value="<?=$_REQUEST['ptoid'] ?>" />
                                    <input type="hidden" name="acilocalizador"  value="<?=$acilocalizador ?>" />

                                    <? if($habil=="S"){ ?>
                                        <input type="button" name="botao" value="Salvar" onclick="salvarAcao('<?=$acao ?>',this);"/>
                                    <? } ?>

                                    <?if($acao=="not"){ ?>
                                    <input type="button" name="botao" value="SubAções" align="right" onClick="paginaSubacao('<?=$aciId ?>');"/>
                                    <? } ?>
                                    
                                    <input type="button" name="botao" value="Voltar" align="right" onClick="paginaAnterior();"/>
                                
                                </td>
                            </tr>
                        </tbody>
                    </table>
                </form>
            </td>
        </tr>
    </tbody>
</table>


<br/>

            

<script type="text/javascript">
    function paginaAnterior()
    {
       return history.back();
    }

    function salvarAcao(acao)
    {
        var objdescricao   = "";
        var objresponsavel = "";
        var objcargo       = "";
        var objdtinicial   = "";
        var objdtfinal     = "";
        var objresultado   = "";
        var objValida      = false;

        objdescricao   = document.formulario.acidescricao ? document.formulario.acidescricao : document.formulario.acidescricao_fake;
        objresponsavel = document.formulario.aciresponsavel;
        objcargo       = document.formulario.acicargo;
        objdtinicial   = document.formulario.acidtinicial;
        objdtfinal     = document.formulario.acidtfinal;
        objresultado   = document.formulario.aciresultado;
        var objptoid       = document.formulario.ptoid;
        var objlocalizador = document.formulario.acilocalizador;

        if (trim(objdescricao.value) == "") {
            alert("Informe a descrição da ação!");
        } else if (trim(objresponsavel.value)=="") {
            alert("Informe responsavel!");
        } else if (trim(objcargo.value)=="") {
            alert("Informe o cargo!");
        } else if (trim(objdtinicial.value)=="") {
            alert("Informe data inicial!");
        } else if (trim(objdtfinal.value)=="") {
            alert("Informe data final!");
        } else if (trim(objresultado.value)=="") {
            alert("Informe resultado esperado !");
        } else {

            objValida = verifica_data(objdtinicial.value, "Informe data inicial valida!") &&
                        verifica_data(objdtfinal.value, "Informe data final valida!")     &&
                        maior_data(objdtinicial.value, objdtfinal.value);

        }

        function verifica_data(stringData, msg) {
            dia = (stringData.substring(0,2));
            mes = (stringData.substring(3,5));
            ano = (stringData.substring(6,10));

            // verifica o dia valido para cada mes
            if (((dia <  01) || (dia < 01 || dia > 30) && (mes == 04 || mes == 06 || mes == 09 || mes == 11 ) || dia > 31) ||
                (mes < 01 || mes > 12) ||
                (mes == 2 && ( dia < 01 || dia > 29 || ( dia > 28 && (parseInt(ano / 4) != ano / 4)))))
            {
                alert(msg);
                return false;
            }

            return true;
        }

        function maior_data(menor,maior)
        {
            meDia = (menor.substring(0,2));
            meMes = (menor.substring(3,5));
            meAno = (menor.substring(6,10));
            datamenor = meAno+meMes+meDia;

            maDia = (maior.substring(0,2));
            maMes = (maior.substring(3,5));
            maAno = (maior.substring(6,10));
            datamaior = maAno+maMes+maDia;

            if(datamenor > datamaior){
                alert("data final é menor que data inicial !");
                return false;
            }

            return true;
        }

        if (objValida) {
            document.formulario.action = "../cte/cte.php?modulo=principal/par_acao_pendencias&acao=A&action=" + acao;
            document.formulario.submit();
        }
    }


    function janelaSecundaria (URL) { 
        var janela = window.open(URL,"janelaProposta","width=400,height=300,scrollbars=yes")
        janela.focus(); 
    }

       </script>
</body>
</html>