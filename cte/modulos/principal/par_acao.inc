<?php
cte_verificaSessao();

$habilita         = cte_podeElaborarPlanoDeAcoes($_SESSION['inuid']) && cte_podeEditarParecer( $_SESSION['inuid'] );
$estado_documento = wf_pegarEstadoAtual( cte_pegarDocid( $_SESSION['inuid'] ) );
$habilMunicipio = "N"; // variavel para habilitar se for município deixa ou não o campo descrição da ação habilitado ou não.

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

    return sprintf("%04d-%02d-%02d", $ano, $mes, $dia );
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
    if ($_REQUEST['action'] == 'add' && $_REQUEST['ptoid']) {

        //$checkaciId = $db->pegaUm( "select aci.aciid from cte.acaoindicador aci where aci.ptoid='" . (int) $_REQUEST['ptoid'] . "'");
        $sqlExiste = "
        	select
        		aci.aciid
        	from cte.acaoindicador aci
        	where
        		aci.ptoid = '" . ( (int) $_REQUEST['ptoid'] ) . "' and
        		aci.acilocalizador = '" . ( $_REQUEST['acilocalizador'] ) . "'
        ";
        $checkaciId = $db->pegaUm( $sqlExiste );

        if ($checkaciId == "") {
            if (strlen($_REQUEST['aciresultado']) > 500) {
                echo '<script type="text/javascript">alert("O número máximo de caractéres para o campo Resultado é 500!");history.back();</script>';
            } else {
                $sql = " insert into cte.acaoindicador (
                            ptoid,
                            acidsc,
                            acirpns,
                            acicrg,
                            acidtinicial,
                            acidtfinal,
                            acirstd,
                            acilocalizador,
                            acidata,
                            usucpf,
                            aciparecer
                        ) values ('"
                            .$_REQUEST['ptoid']."','"
                            .$_REQUEST['acidsc']."','"
                            .$_REQUEST['aciresponsavel']."','"
                            .$_REQUEST['acicargo']."',"
                            .$inicio.","
                            .$final.",'"
                            .$_REQUEST['aciresultado']."','"
                            .$_acilocalizador."','"
                            .date("Y-m-d H:i:s")."','"
                            .$_SESSION['usucpf']."','"
                            .$_REQUEST['aciparecer']
                        ."')";

                $db->executar($sql);
                $db->commit();

                $_REQUEST['acao'] = 'A';
                $db->sucesso('principal/estrutura_avaliacao','');
            }
        } else {
            echo '<script type="text/javascript">alert("Já existe acão para este indicador!");history.back();</script>';
        }

        exit;
    } elseif ($_REQUEST['action'] == "remov") {
        $idSubacao = $db->pegaUm("select sbaid from cte.subacaoindicador sac where sac.aciid='".$_REQUEST['aciid']."'");

        if ($idSubacao == "") {
            $sql = " delete from cte.acaoindicador where aciid='".$_REQUEST['aciid']."'";
            $db->executar($sql);
            $db->commit();
            $_REQUEST['acao'] = 'A';
            $db->sucesso('principal/estrutura_avaliacao');
        } else {
            echo '<script type="text/javascript">alert("Não foi possivel excluir ação, existem subações!");window.history.back();</script>';
        }

        exit;
    } elseif ($_REQUEST['action'] == "edit") {
        if (strlen($_REQUEST['aciresultado']) > 500) {
            echo '<script type="text/javascript">alert("O número máximo de caractéres para o campo Resultado é 500!");history.back();</script>';
        } else {
			if( $_REQUEST['aciresponsavel'] && $_REQUEST['acicargo'] && $_REQUEST['aciresultado'] ){        	
	            $sql = "update cte.acaoindicador set
	                        acirpns         = '".$_REQUEST['aciresponsavel']."',
	                        acidsc			= '".$_REQUEST['acidsc']."',
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
	            $db->sucesso('principal/par_acao', '&aciid=' . $_REQUEST['aciid']);
			}
			else{
				echo '<script type="text/javascript">'
	            .'alert("Ocorreu um problema ao tentar gravar os dados.");'
	            .'history.go(-1)'
	            .'</script>';
			}
        }

        exit;
    }
    elseif ($_REQUEST['action'] == 'excluirSubAcao' && $_REQUEST['sbaid'] != '') {
		$sql = '
	        SELECT
	            COUNT(sbaid)
	        FROM
	            cte.subacaoparecertecnico spt
	        WHERE
	            sbaid = ' . $_REQUEST['sbaid'] . ' AND
	            (
	                ssuid IS NOT NULL or
	                trim(sptparecer) <> \'\'
	            )';
	
		$parec = $db->pegaUm($sql);
	
		$sql = '
	        SELECT
	            COUNT(sac.sbaid)
	        FROM
	            cte.subacaoconvenio sac
	        INNER JOIN
	            cte.subacaoindicador sba ON sba.sbaid = sac.sbaid
	        WHERE
	            sac.sbaid = ' . $_REQUEST['sbaid'];
	
		$conv  = $db->pegaUm($sql);
	
		if ($parec >= 1 || $conv >= 1) {
	            $msg = '<script type="text/javascript">'
	                  .'alert("Operação não realizada!\nFoi dado o parecer técnico ou foi gerado convênio.");'
	                  .'window.location.href = "/brasilpro/brasilpro.php?modulo=principal/par_acao&acao=A&aciid='. $_REQUEST['aciid'] .'"'
	                  .'</script>';
	        } else {
	        $db->executar('
	        			   DELETE FROM
	                        cte.subacaoparecertecnico
	                       WHERE
	                        sbaid = ' . (integer) $_REQUEST['sbaid'] .';
	
	        			   DELETE FROM
	                        cte.subacaobeneficiario
	                       WHERE
	                        sbaid = ' . (integer) $_REQUEST['sbaid'] .';
	
	                       DELETE FROM
	                        cte.composicaosubacao
	                       WHERE
	                        sbaid = ' . (integer) $_REQUEST['sbaid'] .';
	
	                       DELETE FROM
	                        cte.qtdfisicoano
	                       WHERE
	                        sbaid = ' . (integer) $_REQUEST['sbaid'] .';
	
	                       DELETE FROM
	                        cte.termosubacaoindicador
	                       WHERE
	                        sbaid = ' . (integer) $_REQUEST['sbaid'] .';
	
	                       DELETE FROM
	                        cte.subacaoindicador
	                       WHERE
	                        sbaid = ' . (integer) $_REQUEST['sbaid']);
	
	        $db->commit();
	
	        $msg = '<script type="text/javascript">'
	                  .'alert("Subação excluída com sucesso.");'
	                  .'window.location.href = "/cte/cte.php?modulo=principal/par_acao&acao=A&aciid='. $_REQUEST['aciid'] .'"'
	                  .'</script>';
		}
	
		echo $msg;
		die();
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
$acidsc   		= $acaoIndicador['acidsc'];
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

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

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
    /*
    if ( cte_pegarItrid( $_SESSION['inuid'] ) == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ) {
        if ( cte_verificaGrandeMunicipio($_SESSION['inuid']) ) {
            $habilProposta = $habil;
        } else {
            $habilProposta = "N";
        }
    }
    */
    $habilProposta = $habil;
     if ( cte_possuiPerfil( array( CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR ))  ) {
    	$habilMunicipio = "S";
     }else{
     	$habilMunicipio = "N";
     }
} else {
    $habilProposta = "S";
}

//		if(!cte_verificaGrandeMunicipio($_SESSION['inuid'])){
//			$habilProposta = "N";
//		}else{
//			$habilProposta = $habil;
//		}

//echo "mostra mostra lista:".$mostraLista;

$db->cria_aba( $abacod_tela, $url, '&indid='. $indicadorId);
cte_montaTitulo( $titulo_modulo );
?>

<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>

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
	                            <td><? 
	                            		if(cte_pegarItrid( $_SESSION['inuid'] ) === INSTRUMENTO_DIAGNOSTICO_ESTADUAL){ 
	                            			echo campo_texto( 'acidsc', 'S', $habil, '', 50, 100, '', '' );
			                            }else if(cte_pegarItrid( $_SESSION['inuid'] ) === INSTRUMENTO_DIAGNOSTICO_MUNICIPAL){
			                            	echo campo_texto( 'acidsc', 'S', $habilMunicipio, '', 50, 100, '', '' );
			                            }
	                            	?>
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

							<!-- 	                            
                            <?php //if ( cte_podeEditarParecer( $_SESSION['inuid'] ) || !empty( $aciparecer ) ): ?>
                                <tr>
                                    <td align='right' class="SubTituloDireita">Parecer:</td>
                                    <td>
                                    <?php
                                    /*
                                        $habil_parecer = cte_podeEditarParecer( $_SESSION['inuid'] ) ? 'S' : 'N'; 
                                        campo_textarea( 'aciparecer', 'S', $habil_parecer, '', 70, 3, 0 );
									*/
                                    ?>
                                    </td>
                                </tr>
                            <?php// endif; ?>
                            -->
                            
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
<?php if ( $_REQUEST['aciid'] ) :  ?>
    <?
//		$podeManipularSubacao =
//			$habil == 'S' &&
//			(
//				true // verificação realizada no desenho
//				//!cte_pegarMuncod( $_SESSION['inuid'] ) // || // não pode ser município OU
//				//cte_verificaGrandeMunicipio( $_SESSION['inuid'] ) // tem que ser grande município
//			);
        //$podeManipularSubacao = cte_podeEditarSubacao( $_SESSION['inuid'] );
        /*
        $podeManipularSubacao = false;
        echo "testes". $estado_documento['esdid'];
        
        if( ( $estado_documento['esdid'] == CTE_ESTADO_PAR || 
              $estado_documento['esdid'] == CTE_ESTADO_DIAGNOSTICO  
             ) && 
              cte_possuiPerfil( array( CTE_PERFIL_SUPER_USUARIO, 
                                       CTE_PERFIL_ADMINISTRADOR, 
                                       CTE_PERFIL_EQUIPE_MUNICIPAL ) 
                               )
          )
          {
              $podeManipularSubacao = true;
          }

echo "pode?" .  $podeManipularSubacao;     
*/
           
    ?>
               <!--  Lista de Ações Cadastradas -->
               <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
                       <tbody>
                               <tr>
                                       <td class="SubtituloEsquerda">Lista de Subações Cadastradas Nesta Ação</td>
                               </tr>
                       </tbody>
               </table>
               <?php
               
               $arAno = cte_recuperArArAno();
               
				$sql = "select sbaid, sbadsc, frmdsc, unddsc 
						from cte.subacaoindicador s
							left join cte.formaexecucao f on f.frmid=s.frmid
							left join cte.unidademedida u on s.undid=u.undid 
						where s.aciid= '".$_REQUEST['aciid'] ."' 
						order by s.sbadsc";
				
				$res = $db->carregar( $sql );

				if( is_array( $res ) ){ ?>
						
					<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
						<thead>
							<tr align="center">
								<td width="5%"><strong>Ação</strong></td>
								<td width="50%"><strong>Nome da Subação</strong></td>
								<td width="20%"><strong>Forma de Execução</strong></td>
								<td width="20%"><strong>Unid. Medida</strong></td>
								<?php foreach( $arAno as $ano ){
									echo "<td><strong>$ano</strong></td>";	
								} ?>
							</tr>
						</thead>
						<?php 
						$bgColor = "#f7f7f7";
							
						foreach( $res as $dados ){
							
							if( cte_possuiSubacaoPorEscola( $dados["sbaid"] ) ){
								
								$sql = "select qfaano as ano, sum( qfaqtd ) as qtd
										from cte.qtdfisicoano 
										where sbaid = ". $dados["sbaid"] ."
										group by qfaano";
							}
							else{
								$sql = "select sptano as ano, sptunt as qtd 
										from cte.subacaoparecertecnico
										where sbaid = ". $dados["sbaid"];
							}	
							
							$res = $db->carregar( $sql );
							
							$arCronograma = array();
							
							if( is_array( $res ) ){
								foreach( $res as $arQtd ){
									$arCronograma[$arQtd["ano"]] = $arQtd["qtd"];
								}	
							}	
							$bgColor = $bgColor == "#ffffff" ? "#f7f7f7" : "#ffffff";
						?>
							<tr bgcolor="<?php echo $bgColor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $bgColor ?>';">
								<td>
									<a href="javascript:editar_par( '<?php echo $dados["sbaid"] ?>', '<?php echo $_REQUEST['aciid'] ?>' );">
										<img src="/imagens/alterar.gif" border=0 title="Selecionar">
									</a>&nbsp;
			                       
			                       <?php if( ( $estado_documento['esdid'] == CTE_ESTADO_ANALISE ||  $estado_documento['esdid'] == CTE_ESTADO_DIAGNOSTICO  ) && 
			                             		cte_possuiPerfil( array( CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR, CTE_PERFIL_EQUIPE_MUNICIPAL ) )
			                         		)
			                         { ?>									
										<a href="javascript: excluirSubacao( '<?php echo $dados["sbaid"] ?>', '<?php echo $_REQUEST['aciid'] ?>' );">
											<img src="/imagens/excluir.gif" border=0 title="Excluir">
										</a>
								<?php } ?>
								</td>
								<td><?php echo $dados["sbadsc"] ?></td>
								<td><?php echo $dados["frmdsc"] ?></td>
								<td><?php echo $dados["unddsc"] ?></td>
								<?php foreach( $arAno as $ano ){
									$qtd = array_key_exists( $ano, $arCronograma ) ? $arCronograma[$ano] : 0;	
									echo "<td align='right'>$qtd</td>";
								} ?>
							</tr>
						<?php } ?>
					</table>	
				<?php } ?>

				<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
					<tbody>
						<tr>
							<td class="SubtituloDireita">
								<?php
								                                       
								// verifica se é município e pequeno município
								$pequenoMunicipio = cte_pegarMuncod( $_SESSION['inuid'] ) && !cte_verificaGrandeMunicipio( $_SESSION['inuid'] );
								                                        
								$equipeTecnicaMec = cte_possuiPerfil( array( CTE_PERFIL_EQUIPE_TECNICA ) );
								$superUser = $db->testa_superuser();
								                                       
								?>
								<?php if ( $podeManipularSubacao ): ?>
									<input type="button" name="Adicionar Nova Subação" value="Adicionar Nova Subação" onclick="adiciar_par()"/>
								<?php endif; ?>
							</td>
						</tr>
					</tbody>
				</table>
       <?php endif; ?>

<script type="text/javascript">
    function editar_par(id,localizador)
    {
        var janela = window.open( "?modulo=principal/par_subacao&acao=A&sbaid=" + id, 'blank', 'height=600,width=900,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );
        janela.focus();
    }

    function adiciar_par()
    {
        var janela = window.open( "?modulo=principal/par_subacao&acao=A&aciid=<?= $_REQUEST['aciid'] ?>", 'blank', 'height=600,width=900,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );
        janela.focus();
    }

    function excluir_par( sbaid )
    {
        if ( confirm( "Deseja excluir a subação?" ) )
        {
            window.location = "?modulo=principal/par_subacao&acao=A&sbaid=" + sbaid + "&action=remov&origem=<?= urlencode( $_SERVER['QUERY_STRING'] ) ?>";
        }
    }

	function excluirSubacao( sbaid, acao ){
		if( confirm( "Deseja excluir a subação?" ) ){
			window.location = "/cte/cte.php?modulo=principal/par_acao&acao=A&aciid=" + acao + "&sbaid=" + sbaid + "&action=excluirSubAcao";
		}	
	}

    function paginaAnterior()
    {
       return history.back();
    }

    function paginaSubacao(acao)
    {
        window.open( "?modulo=principal/par_subacao&acao=A&aciid="+acao, 'blank', 'height=600,width=900,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );
    }

    function salvarAcao(acao)
    {
        var objresponsavel = "";
        var objcargo       = "";
        var objdtinicial   = "";
        var objdtfinal     = "";
        var objresultado   = "";
        var objValida      = false;

        objresponsavel = document.formulario.aciresponsavel;
        objcargo       = document.formulario.acicargo;
        objdtinicial   = document.formulario.acidtinicial;
        objdtfinal     = document.formulario.acidtfinal;
        objresultado   = document.formulario.aciresultado;
        var objptoid       = document.formulario.ptoid;
        var objlocalizador = document.formulario.acilocalizador;

        if (trim(objresponsavel.value)=="") {
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
            document.formulario.action = "../cte/cte.php?modulo=principal/par_acao&acao=A&action=" + acao;
            document.formulario.submit();
        }
    }


    function janelaSecundaria (URL) { 
        var janela = window.open(URL,"janelaProposta","width=400,height=300,scrollbars=yes")
        janela.focus(); 
    }

       </script>