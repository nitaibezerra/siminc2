<?php

if( $_REQUEST['act'] == 'filtraPlanoOrcamentario'){

	//recuperando o $acaid
	if ($_REQUEST['prgcod'] && $_REQUEST['acacod'] && $_REQUEST['unicod'] && $_REQUEST['loccod']){
		$acaid = $db->pegaUm( sprintf( "SELECT acaid FROM monitora.acao WHERE acasnrap = false AND prgano = '%s' AND prgcod = '%s' AND acacod = '%s' AND unicod = '%s' AND loccod = '%s'"
									, $_SESSION['exercicio']
									, $_REQUEST['prgcod']
									, $_REQUEST['acacod']
									, $_REQUEST['unicod']
									, $_REQUEST['loccod']
									)
								);
	}

	$modelo = new PlanoOrcamentario();
	$dados  = $modelo->recuperarTodos("ploid AS codigo, plocod || ' - ' || plonome AS descricao", array("plostatus = 'A'","acaid = {$acaid}"), "descricao");
    $db->monta_combo("ploid", $dados, 'S', '&nbsp', '', '', '', '570', 'S', 'ploid');

	die();
}


if( $_REQUEST['act'] == 'validaPrioridade'){

	$retorno = 0;

	if( !empty( $_REQUEST['prioridade'] ) ){
		$projetoPlanejamento = new ProjetoPlanejamento();
		$retorno = count($projetoPlanejamento->validaPrioridade( $_REQUEST['prioridade'] , $_REQUEST['unidade'], $_REQUEST['pplid']));
	}

	echo $retorno;
	die();
}

if( $_REQUEST['act'] == 'atualizaPrioridades'){


	if( !empty( $_REQUEST['prioridade'] ) ){
		$projetoPlanejamento = new ProjetoPlanejamento();
		$retorno = $projetoPlanejamento->validaPrioridade( $_REQUEST['prioridade'] , $_REQUEST['unidade'], $_REQUEST['pplid']);

		if( is_array($retorno) && !empty($retorno)){
			foreach ($retorno as $dado){
				$dados 		  			= array();
				$dados['pplid'] 		= (int) $dado;
				$dados['pplprioridade'] = null;
				$nulos[]				= pplprioridade;

				$projetoPlanejamento->popularDadosObjeto($dados)->salvar(null,null, $nulos);
				$projetoPlanejamento->setDadosNull();
			}
			$projetoPlanejamento->commit();
		}
	}
	echo 'true';
	die();
}

?>

<script type="text/javascript" src="../includes/prototype-1.6.0.3.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/micoxAjax.js"></script>
<script type="text/javascript">
	jQuery.noConflict();
</script>

<?php

$permissao_formulario = 'S';

$arMes = array('1'  => 'Janeiro',
               '2'  => 'Fevereiro',
               '3'  => 'Março',
               '4'  => 'Abril',
               '5'  => 'Maio',
               '6'  => 'Junho',
               '7'  => 'Julho',
               '8'  => 'Agosto',
               '9'  => 'Setembro',
               '10' => 'Outubro',
               '11' => 'Novembro',
               '12' => 'Dezembro');

if($_REQUEST['act'] == 'gravar'){

	$projetoPlanejamento = new ProjetoPlanejamento();

    $arCamposNulos = array();
    if($_REQUEST['uexid'] == ''){
        $_REQUEST['uexid'] = null;
        $arCamposNulos[]   = 'uexid';
    }

    if($_REQUEST['tppid'] == ''){
        $_REQUEST['tppid'] = null;
        $arCamposNulos[]   = 'tppid';
    }

    if($_REQUEST['esfid'] == ''){
        $_REQUEST['esfid'] = null;
        $arCamposNulos[]   = 'esfid';
    }

    if( !$_REQUEST['paiid'] ){
		$_REQUEST['paiid'] = 1; //valor do Brasil
    }else{
    	if($_REQUEST['paiid'] == ''){
       		$_REQUEST['paiid'] = null;
    	    $arCamposNulos[]   = 'paiid';
	    }
    }

    if($_REQUEST['estuf'] == ''){
        $_REQUEST['estuf'] = null;
        $arCamposNulos[]   = 'estuf';
    }

    if($_REQUEST['muncod'] == ''){
        $_REQUEST['muncod'] = null;
        $arCamposNulos[]   = 'muncod';
    }

	if ( $_REQUEST['estuf'] == 'DF' ){
		$codigoMunicipio = "5300108"; // Brasilia
		$codigoRegiaoAdministrativa = $_REQUEST['muncod'];
	}
	else{
		$codigoMunicipio = $_REQUEST['muncod'];
		$codigoRegiaoAdministrativa = null;
        $arCamposNulos[]   = 'rgaid';
	}

	//recuperando o $acaid
	if ($_REQUEST['prgcod'] && $_REQUEST['acacod'] && $_REQUEST['unicod'] && $_REQUEST['loccod']){
		$acaid = $db->pegaUm( sprintf( "SELECT acaid FROM monitora.acao WHERE acasnrap = false AND prgano = '%s' AND prgcod = '%s' AND acacod = '%s' AND unicod = '%s' AND loccod = '%s'"
									, $_SESSION['exercicio']
									, $_REQUEST['prgcod']
									, $_REQUEST['acacod']
									, $_REQUEST['unicod']
									, $_REQUEST['loccod']
									)
								);
	}

    $arDados = array();
    $arDados = array(
    				  'pplid'					=> (int) $_REQUEST['pplid'],
                      'uexid'          			=> $_REQUEST['uexid'],
                      'tppid'          			=> $_REQUEST['tppid'],
                      'pplprioridade'         	=> $_REQUEST['pplprioridade'],
                      'ppltitulo'          		=> $_REQUEST['ppltitulo'],
                      'pplobjetivo'          	=> $_REQUEST['pplobjetivo'],
                      'ppljustificativa'        => $_REQUEST['ppljustificativa'],
                      'ppldescricao'          	=> $_REQUEST['ppldescricao'],
                      'pploutrasiniciativas'	=> $_REQUEST['pploutrasiniciativas'],
                      'pplvlrcusteio'  			=> $_REQUEST['pplvlrcusteio'] ? str_replace(array('.', ','), array('', '.'), $_REQUEST['pplvlrcusteio']) : $_REQUEST['pplvlrcusteio'],
                      'pplvlrcapital'  			=> $_REQUEST['pplvlrcapital'] ? str_replace(array('.', ','), array('', '.'), $_REQUEST['pplvlrcapital']) : $_REQUEST['pplvlrcapital'],
                      'esfid'					=> $_REQUEST['esfid'],
                      'paiid'					=> $_REQUEST['paiid'],
                      'estuf'					=> $_REQUEST['estuf'],
                      'muncod'					=> $codigoMunicipio,
    				  'acaid'					=> $acaid ? $acaid : null,
                      'rgaid'					=> $codigoRegiaoAdministrativa,
    				  'pplprosecundariometa'	=> $_REQUEST['pplprosecundariometa'],
					  'pduid'					=> $_REQUEST['pduid'],
					  'udaid'					=> $_REQUEST['udaid'],
					  'ploid'					=> $_REQUEST['ploid'],
					  'ploflgappcs'			    => $_REQUEST['ploflgappcs'] ? $_REQUEST['ploflgappcs'] : 'f'
                    );


	if(empty($_REQUEST['pplid'])){
	   $arDados['docid']		= wf_cadastrarDocumento( WF_PLANEJAMENTO, 'Planejamento' );
	   $arDados['pplstatus']	= 'A';
	   $arDados['prsano']		= $_SESSION['exercicio'];
	}

    $pplid    = $projetoPlanejamento->popularDadosObjeto($arDados)->salvar(null, null, $arCamposNulos);

    $cFisico       = new CronogramaFisico();
    $cOrcamentario = new CronogramaOrcamentario();
    $cFinanceiro   = new CronogramaFinanceiro();
    $projetoIni    = new ProjetoIniciativa();
    $projetoCon    = new ProjetoContratacao();

    $cFisico->apagaPorPlanejamento($pplid);
    $cOrcamentario->apagaPorPlanejamento($pplid);
    $cFinanceiro->apagaPorPlanejamento($pplid);
    $projetoIni->apagaPorPlanejamento($pplid);
    $projetoCon->apagaPorPlanejamento($pplid);

    $_REQUEST['pplid'] = $pplid;

    foreach ($arMes as $numMes => $nomeMes){
        if($_REQUEST["crfprevistomes{$numMes}"]){
            $_REQUEST["crfprevistomes{$numMes}"] = str_replace(array('.', ','), array('', '.'), $_REQUEST["crfprevistomes{$numMes}"]);
        }else{
            $_REQUEST["crfprevistomes{$numMes}"] = null;
        }


        if($_REQUEST["croprevistocusteiomes{$numMes}"]){
            $_REQUEST["croprevistocusteiomes{$numMes}"] = str_replace(array('.', ','), array('', '.'), $_REQUEST["croprevistocusteiomes{$numMes}"]);
        }else{
            $_REQUEST["croprevistocusteiomes{$numMes}"] = null;
        }
        if($_REQUEST["croprevistocapitalmes{$numMes}"]){
            $_REQUEST["croprevistocapitalmes{$numMes}"] = str_replace(array('.', ','), array('', '.'), $_REQUEST["croprevistocapitalmes{$numMes}"]);
        }else{
            $_REQUEST["croprevistocapitalmes{$numMes}"] = null;
        }


        if($_REQUEST["cfiprevistocusteiomes{$numMes}"]){
            $_REQUEST["cfiprevistocusteiomes{$numMes}"] = str_replace(array('.', ','), array('', '.'), $_REQUEST["cfiprevistocusteiomes{$numMes}"]);
        }else{
            $_REQUEST["cfiprevistocusteiomes{$numMes}"] = null;
        }
        if($_REQUEST["cfiprevistocapitalmes{$numMes}"]){
            $_REQUEST["cfiprevistocapitalmes{$numMes}"] = str_replace(array('.', ','), array('', '.'), $_REQUEST["cfiprevistocapitalmes{$numMes}"]);
        }else{
            $_REQUEST["cfiprevistocapitalmes{$numMes}"] = null;
        }
    }

    if( is_array( $_REQUEST['vinculacaoPlanejamento'] ) && !empty( $_REQUEST['vinculacaoPlanejamento'] ) ){
    	foreach( $_REQUEST['vinculacaoPlanejamento'] as $vinculacaoPlanejamento ){
			$dadosVinculacaoPlanejamento 		  = array();
			$dadosVinculacaoPlanejamento['pplid'] = (int) $pplid;
			$dadosVinculacaoPlanejamento['iteid'] = (int) $vinculacaoPlanejamento;

			$projetoIni->popularDadosObjeto($dadosVinculacaoPlanejamento)->salvar();
			$projetoIni->setDadosNull();

    	}
    }

    if( (is_array( $_REQUEST['forma'] ) && !empty($_REQUEST['forma'])) && (is_array( $_REQUEST['objeto'] ) && !empty($_REQUEST['objeto'])) && (is_array( $_REQUEST['valorcusteio'] ) && !empty($_REQUEST['valorcapital'])) && (is_array( $_REQUEST['valorcapital'] ) && !empty($_REQUEST['valorcapital'])) ){
    	foreach( $_REQUEST['forma'] as $chave => $forma ){
			$dadosProjetoContratacao 		  			= array();
			$dadosProjetoContratacao['pplid'] 			= (int) $pplid;
			$dadosProjetoContratacao['itrid'] 			= (int) $forma;
			$dadosProjetoContratacao['prcnome']			= $_REQUEST['objeto'][$chave];
			$dadosProjetoContratacao['prcvalorcusteio']	= $_REQUEST['valorcusteio'][$chave] ? str_replace(array('.', ','), array('', '.'), $_REQUEST['valorcusteio'][$chave]) : $_REQUEST['valorcusteio'][$chave];
			$dadosProjetoContratacao['prcvalorcapital']	= $_REQUEST['valorcapital'][$chave] ? str_replace(array('.', ','), array('', '.'), $_REQUEST['valorcapital'][$chave]) : $_REQUEST['valorcapital'][$chave];

			$projetoCon->popularDadosObjeto($dadosProjetoContratacao)->salvar();
			$projetoCon->setDadosNull();
    	}
    }

    $cOrcamentario->popularDadosObjeto()->salvar();
    $cFinanceiro->popularDadosObjeto()->salvar();
    $cFisico->popularDadosObjeto()->salvar();

	//INSERE ABRANGENCIA
	$sql = "DELETE FROM sisplan.projetoregionalizacao WHERE pplid = '". $pplid ."'";
	$db->executar($sql);

	if( $_REQUEST['atisndetalharregionalizacao'] == 'true'){
		$abrpais = is_array($_REQUEST['pais']) ? $_REQUEST['pais'] : array();
		foreach ( $abrpais as $k=>$pais )
		{
			if ( $pais != "" )
			{
				$sql   = "SELECT paiid FROM territorios.pais WHERE paidescricao = '". $pais ."'";
				$paiid = $db->pegaUm($sql,0);

				$arrayuf = explode(" - ", $_REQUEST['uf'][$k]);
				$estuf   = $arrayuf[0];


				if ( $estuf == 'DF' )
				{
					$sql      = "select rgaid from planointerno.regiaoadministrativa where rgadsc = '". $_REQUEST['mun'][$k] ."'";
					$rgaid    = $db->pegaUm($sql,0);
					$estufmun = '';
					$muncod   = '';
				}
				else
				{
					$arraymun = explode(" - ", $_REQUEST['mun'][$k]);
					$sql      = "select muncod from territorios.municipio where estuf = '". $arraymun[0] ."' and mundescricao = '". $arraymun[1] ."'";
					$muncod   = $db->pegaUm($sql);
				}

				$sql = sprintf(
					"INSERT INTO sisplan.projetoregionalizacao
						( pplid, paiid, estuf, muncod, rgaid )
					 VALUES
						( %s, %s, %s, %s, %s )",
					$pplid,
					$paiid,
					$estuf ? "'".$estuf."'" : 'NULL',
					$muncod ? "'".$muncod."'" : 'NULL',
					$rgaid ? $rgaid : 'NULL' );

				$db->executar( $sql );
			}
		}
	}

    $db->commit();
    $db->sucesso( 'principal/consultarPlanejamento');
    die();
}

include  APPRAIZ."includes/cabecalho.inc";
echo "<br />";

$unidadeExecutora	= new UnidadeExecutora();
$responsabilidades	= $unidadeExecutora->getResponsabilidadeUnidadesByUsuario();

$excecao 	= new ExcecaoUnidade();
$excecoes	= $excecao->verificaExcecao(null, true);

if($_REQUEST['pplid']){
    $titulo_modulo = 'Alterar Planejamento';

    $modelo = new ProjetoPlanejamento($_REQUEST['pplid']);

    $pplid = (int) $_REQUEST['pplid'];

    $dados = $modelo->getDados();

    if ( !$dados['docid']){
    	$arDados 		  = array();
		$arDados['docid'] = wf_cadastrarDocumento( WF_PLANEJAMENTO, 'Planejamento' );

    	$modelo->popularDadosObjeto($arDados)->salvar();
    	$modelo->commit();
    }

    extract($modelo->getDados());

    $pplid 				= (int)$pplid;
	$_REQUEST['pplid']	= $pplid;

	if ($acaid){
		$dados_acaid = $db->pegaLinha( sprintf( "SELECT a.prgcod, a.acacod, a.unicod, a.loccod, u.unmdsc, p.prodsc FROM monitora.acao a LEFT JOIN public.produto
		p ON p.procod = a.procod LEFT JOIN public.unidademedida u ON u.unmcod = a.unmcod
		WHERE acaid = '%s'", $acaid ) );

		extract($dados_acaid);
	}

    $muncod = !empty($rgaid) ? $rgaid : $muncod;

    $pplvlrcusteio = formata_valor($pplvlrcusteio);
    $pplvlrcapital = formata_valor($pplvlrcapital);

    $cFisico       = new CronogramaFisico();
    $cOrcamentario = new CronogramaOrcamentario();
    $cFinanceiro   = new CronogramaFinanceiro();

    $dadosFisico       = $cFisico->pegaPorPlanejamento($pplid);
    $dadosOrcamentario = $cOrcamentario->pegaPorPlanejamento($pplid);
    $dadosFinanceiro   = $cFinanceiro->pegaPorPlanejamento($pplid);
    
    if($dadosFisico){
        foreach ($dadosFisico as $k => $value){
            if(($k != 'crfid' && $k != 'ppiid') && !is_null($value) )
                $dadosFisico[$k] = intVal($value);
        }
        extract($dadosFisico);
    }


    if($dadosOrcamentario){
        foreach ($dadosOrcamentario as $k => $value){
            if(($k != 'croid' && $k != 'ppiid') && !is_null($value) )
                $dadosOrcamentario[$k] = formata_valor($value);
        }
        extract($dadosOrcamentario);
    }

    if($dadosFinanceiro){
        foreach ($dadosFinanceiro as $k => $value){
            if(($k != 'cfiid' && $k != 'ppiid') && !is_null($value) )
                $dadosFinanceiro[$k] = formata_valor($value);
        }
        extract($dadosFinanceiro);
    }

    //verificando as responsabilidades para bloquear a tela
    if( !$db->testa_superuser() &&
		!possuiPerfil(PERFIL_APOIO_COORDENADOR_ORCAMENTO) &&
		!possuiPerfil(PERFIL_APOIO_COORDENADOR_PLANEJAMENTO) &&
		!possuiPerfil(PERFIL_COORDENADOR_ORCAMENTO) &&
		!possuiPerfil(PERFIL_COORDENADOR_PLANEJAMENTO) ){
	    if( is_array( $responsabilidades ) &&  !empty( $responsabilidades) ){
	    	if ( in($uexid, $responsabilidades) ){
	    		$permissao_formulario = 'S';
	    	}else{
		    	$permissao_formulario = 'N';
	    	}
	    }else{
	    	$permissao_formulario = 'N';
	    }
    }

	//Verificando a data de inicio e termino do planejamento ( definida na programacaoexercicio )
    $programacaoExercicio = new ProgramacaoExercicio();
	$permissao = $programacaoExercicio->verificaPermissaoPlanejamento();

	if( !$permissao ){
		//verifica a liberação de exceção
		$excecaoUnidade 	= new ExcecaoUnidade();
		$excecao			= $excecaoUnidade->verificaExcecao($dados['uexid']);

		if( !$excecao ){
	    	$permissao_formulario = 'N';
		}else{
	    	$permissao_formulario = 'S';
		}
	}

}

monta_titulo( $titulo_modulo, obrigatorio() . 'Indica Campo Obrigatório.' );
?>

<form action="" method="post" name="formulario">
    <input type="hidden" id="pplid" name="pplid" value="<?= $pplid; ?>"/>
    <input type="hidden" name="act" id="act" value="gravar" />
    <input type="hidden" name="hdn_esfid" id="hdn_esfid" value="<?php echo $esfid; ?>" />
    <input type="hidden" name="hdn_paiid" id="hdn_paiid" value="<?php echo $paiid; ?>" />
    <input type="hidden" name="hdn_estuf" id="hdn_estuf" value="<?php echo $estuf; ?>" />
    <input type="hidden" name="hdn_muncod" id="hdn_muncod" value="<?php echo $muncod; ?>" />

    <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Unidade Executora:
            </td>
            <td>
                <?php

                if( !$db->testa_superuser() &&
					!possuiPerfil(PERFIL_APOIO_COORDENADOR_ORCAMENTO) &&
					!possuiPerfil(PERFIL_APOIO_COORDENADOR_PLANEJAMENTO) &&
					!possuiPerfil(PERFIL_COORDENADOR_ORCAMENTO) &&
					!possuiPerfil(PERFIL_COORDENADOR_PLANEJAMENTO) ){
						if(is_array( $responsabilidades ) && !empty( $responsabilidades )){
							$cond = " uexid IN ('".implode("','", $responsabilidades)."') ";
						}else{
							$cond = " FALSE ";
						}

						$programacaoExercicio = new ProgramacaoExercicio();
						$permissao_exercicio = $programacaoExercicio->verificaPermissaoPlanejamento();

						if ( !$permissao_exercicio ){

							if(is_array( $excecoes ) && !empty( $excecoes )){
								$exce = " uexid IN ('".implode("','", $excecoes)."') ";
							}else{
								$exce = " FALSE ";
							}

							$operador = " AND ";

							//se for alteração
							if($_REQUEST['pplid']){
								//verifica a liberação de exceção
								$excecaoUnidade 	= new ExcecaoUnidade();
								$excecao			= $excecaoUnidade->verificaExcecao($dados['uexid']);

								if( !$excecao ){
							    	$operador = " OR ";
								}
							}

							$cond = " ( ". $cond .$operador.$exce ." ) ";
						}

					}else{
						$cond = " TRUE ";
					}



                    $modelo = new UnidadeExecutora();
					$dados  = $modelo->recuperarTodos("uexid AS codigo, uexsigla || ' - ' || uexdsc AS descricao", array("uexstatus = 'A'",$cond), "descricao");
                    $db->monta_combo("uexid", $dados, $permissao_formulario, '&nbsp', '', '', '', '517', 'S', 'uexid');
                ?>
            </td>
            <td rowspan="7" colspan="2">
                <?php
                    if( $pplid && $permissao_formulario == 'S' ){
                        wf_desenhaBarraNavegacao( $docid, array("pplid" => $pplid) );
                    }
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Tipo de Projeto:
            </td>
            <td>
                <?php
                    $modelo = new TipoProjetoPlanejamento();
					$dados  = $modelo->recuperarTodos("tppid AS codigo, tppnome || ' - ' || tppdescricao AS descricao", array("tppstatus = 'A'"), "descricao");
                    $db->monta_combo("tppid", $dados, $permissao_formulario, '&nbsp', '', '', '', '517', 'S', 'tppid');
                ?>
            </td>
        </tr>
        <tr class="nome-descricao" style="<?= $style; ?>">
            <td align='right' class="SubTituloDireita" style="width:25%;">
                 Prioridade:
            </td>
            <td colspan="5">
                 <?= campo_texto('pplprioridade', 'S', $permissao_formulario, '', 10, 4, '####', '','','','','id="pplprioridade"','','',''); ?>
            </td>
        </tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
			     Título (Definição do Objeto):
		    </td>
            <td>
                <?= campo_textarea( 'ppltitulo', 'S', $permissao_formulario, '', 95, 3, 200 ); ?>
            </td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
			     Objetivos (Para que?):
		    </td>
            <td>
                <?= campo_textarea( 'pplobjetivo', 'S', $permissao_formulario, '', 95, 5, 2000 ); ?>
            </td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
			     Justificativa (Por que?):
		    </td>
            <td>
                <?= campo_textarea( 'ppljustificativa', 'S', $permissao_formulario, '', 95, 5, 2000 ); ?>
            </td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
			     Descrição das Atividades (Como?):
		    </td>
            <td>
                <?= campo_textarea( 'ppldescricao', 'S', $permissao_formulario, '', 95, 5, 2000 ); ?>
            </td>
		</tr>
		<tr>
		<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Vinculação ao Planejamento Estratégico:</td>
			<td>
				<?php
					if( $pplid )
					{
						$sql_vinculacaoPlanejamento = " select
															ite.iteid as codigo,
															dfe.dfedsc || ' - ' || ite.itedsc as descricao,
															dfeobservacao as titulo
														from
															sisplan.iniciativaestrategica ite
														inner join
															sisplan.projetoiniciativa pin on pin.iteid = ite.iteid
														inner join
															sisplan.desafioestrategico dfe on dfe.dfeid = ite.dfeid
														where
															ite.itestatus = 'A' and
															pin.pplid = '" . (int) $pplid . "'
														order by
															ite.iteid";

						$vinculacaoPlanejamento = $db->carregar( $sql_vinculacaoPlanejamento );
						$vinculacaoPlanejamento = $vinculacaoPlanejamento ? $vinculacaoPlanejamento : array();
					}

						$sql_combo = " select
											ite.iteid as codigo,
											dfe.dfedsc || ' - ' || ite.itedsc as descricao,
											dfeobservacao as titulo
										from
											sisplan.iniciativaestrategica ite
										inner join
											sisplan.desafioestrategico dfe on dfe.dfeid = ite.dfeid
										where
											ite.itestatus = 'A'
										order by
											ite.iteid";

					combo_popup(
						'vinculacaoPlanejamento',
						$sql_combo,
						'Selecione a(s) Iniciativa(s) Estratégica(s)',
						'400x400',
						0,
						array(),
						$permissao_formulario,
						$permissao_formulario,
						true
					);
					echo obrigatorio();
				?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
			     Outras Iniciativas:
		    </td>
            <td>
                <?= campo_textarea( 'pploutrasiniciativas', 'N', $permissao_formulario, '', 95, 5, 2000 ); ?>
            </td>
		</tr>

		<tr>
			<td colspan="7" class="SubTituloCentro" style="background-color: #DCDCDC">Pac:</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
			     Ações presentes nos APPCs <br>(Acordos de preservação do patrimônio cultural):
		    </td>
	        <td>
				<input type="radio" name="ploflgappcs" id="ploflgappcs-t" <?php echo $ploflgappcs == 't' ? ' checked="checked" ' : "" ?> value="t" /> <label for="ploflgappcs-t" style="cursor:pointer;"> Sim</label>
				<input type="radio" name="ploflgappcs" id="ploflgappcs-f" <?php echo ($ploflgappcs == 'f' || empty($ploflgappcs) ) ? ' checked="checked" ' : "" ?> value="f" /> <label for="ploflgappcs-f" style="cursor:pointer;"> Não</label>
	        	<?php echo obrigatorio(); ?>
	        </td>
		</tr>
		<tr>
			<td colspan="7" class="SubTituloCentro" style="background-color: #DCDCDC">Regionalização:</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Localização da ação:</td>
			<td>
				<?php
					$sql = "SELECT esfid AS codigo, esfdsc AS descricao FROM planointerno.esfera";
					$db->monta_combo( "esfid",$sql,$permissao_formulario,"&nbsp;",'action_esfera','', '', '200', 'S', "esfid" );
				?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Pa&iacute;s:</td>
			<td>
				<?php
					$sql = "SELECT paiid AS codigo, paidescricao AS descricao FROM territorios.pais ORDER BY 2";
					$db->monta_combo("paiid",$sql,$permissao_formulario,'&nbsp','','','','200','','paiid');
					//echo  obrigatorio();
				?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Unidade Federativa:</td>
			<td>
				<div id="estuf_on" style="<?php if($atividade['esfid']==1 || $atividade['esfid']==''){ ?>display:none;<?php }else{ ?>display:block;<?php } ?>">
				<?php
					$sql = "SELECT estuf AS codigo, estuf||' - '||estdescricao AS descricao FROM territorios.estado ORDER BY 2";
					$db->monta_combo("estuf",$sql,$permissao_formulario,'&nbsp','listar_municipios','','','','','estuf');
					//echo  obrigatorio();
				?>
				</div>
				<div id="estuf_off" style="color:#909090;">&nbsp;</div>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Município:</td>
			<td>
				<div id="muncod_on" style="<?php if($atividade['esfid']==1 || $atividade['esfid']==2 || $atividade['esfid']==''){ ?>display:none;<?php } else { echo "display:block;"; } ?>">
					<?
						if ( $atividade['estuf'] == 'DF' )
						{
							$sql = "SELECT rgaid, rgaid as codigo, rgadsc as descricao FROM planointerno.regiaoadministrativa ORDER BY 3 ";
						}
						else
						{
							$sql = "SELECT estuf, muncod as codigo, estuf || ' - ' || mundescricao as descricao FROM territorios.municipio WHERE estuf='".$atividade['estuf']."' ORDER BY 3 ";
						}
						$db->monta_combo("muncod",$sql,$permissao_formulario,"&nbsp;",'','','','','','muncod');
					?>
				</div>
				<div id="muncod_off" style="color:#909090;">&nbsp;</div>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Detalhar regionalização:</td>
			<td>
				<input type="checkbox" style="cursor:pointer;" onclick="exibeAbrangencia();" id="atisndetalharregionalizacao" name="atisndetalharregionalizacao" <?php if($permissao_formulario == 'N') { echo ' disabled="disabled"'; } ?> value="true" <?php if($atisndetalharregionalizacao=="t"){ echo " checked "; } ?>/> <label for="atisndetalharregionalizacao" style="cursor:pointer;">Marque esta opção para informar múltiplos valores de regionalização.</label>
			</td>
		</tr>
		<tr id="trabrangencia" style="display:none">
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Abrangência:</td>
			<td>
				<table width="80%" align="rigth" border="0" cellspacing="0" cellpadding="2" class="listagem">
					<thead>
						<tr>
							<td width="30%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>País</strong></td>
							<td width="30%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Unidade Federativa</strong></td>
							<td width="30%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Município</strong></td>
							<td width="10%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
						</tr>
					</thead>
						<tr bgcolor="#f7f7f7">
							<td>
								<div id="pais_on">
									<? $sql = "SELECT paidescricao AS codigo, paidescricao AS descricao FROM territorios.pais ORDER BY 2"; ?>
									<? $db->monta_combo("abrpaiid",$sql,$permissao_formulario,'&nbsp','','','',200,'','abrpaiid'); ?>
								</div>
								<div id="pais_off"></div>
							</td>
							<td>
								<div id="abrestuf_on" >
									<? $sql = "SELECT estuf||' - '||estdescricao AS codigo, estuf||' - '||estdescricao AS descricao FROM territorios.estado ORDER BY 2"; ?>
									<? $db->monta_combo("abrestuf",$sql,$permissao_formulario,'&nbsp','listar_municipios_abrangencia','','',200,'','abrestuf'); ?>
								</div>
								<div id="abrestuf_off"></div>
							</td>
							<td align="right">
								<div id="abrmuncod_on" >
									<?
									//dbg($atividade['estuf']);
										if ( $atividade['abrestuf'] == 'DF' )
										{
											$sql = "SELECT rgaid, rgadsc as codigo, rgadsc as descricao FROM planointerno.regiaoadministrativa ORDER BY 3 ";
										}
										else
										{
											$sql = "SELECT estuf, estuf || ' - ' || mundescricao as codigo, estuf || ' - ' || mundescricao as descricao FROM territorios.municipio WHERE estuf='".$atividade['estuf']."' ORDER BY 3 ";
										}
										$db->monta_combo("abrmuncod",$sql,$permissao_formulario,"&nbsp;",'','','','','','abrmuncod');
									?>
								</div>
								<div id="abrmuncod_off"></div>
							</td>
							<td align="left">
								<a href="javascript:item_abrangencia_adicionar_do_formulario();" title="Incluir" style="margin: 0; padding: 0 6px;">
									<img src="../imagens/gif_inclui.gif" border="0" align="absmiddle"/>
								</a>
							</td>
						</tr>
					<tr>
						<td colspan="4">
							<div class="div_rolagem" id="div_tabela_abrangencia" style="width: 100%; padding: 0; margin: 0; height: 100px;position-y: absolute;">
								<table id="tabela_abrangencia" width="100%" align="left" border="0" cellspacing="0" cellpadding="2">
									<tr valign="bottom">
										<td width="30%" style="height: 0; margin: 0; padding: 0;"></td>
										<td width="32%" style="height: 0; margin: 0; padding: 0;"></td>
										<td width="30%" style="height: 0; margin: 0; padding: 0;"></td>
										<td width="3%" style="height: 0; margin: 0; padding: 0;"></td>
									</tr>
								</table>
							</div>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td colspan="7" class="SubTituloCentro" style="background-color: #DCDCDC">PPA:</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">PTRES:</td>
			<td>
				<?=campo_texto('ptres','N',$permissao_formulario,'',12,6,'######','','','Informe o número PTRES.','','id="ptres"');?>
				<span id="span_ptres"></span>
				<script type="text/javascript">
				$("ptres").observe("blur", function(event) {
				listar_ptres($("ptres").value);});
				</script>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Programa:</td>
			<td><?php
				$sql = sprintf( "SELECT distinct p.prgcod as codigo, p.prgcod || ' - ' || prgdsc AS descricao FROM monitora.programa p INNER JOIN monitora.acao a ON a.prgid = p.prgid WHERE a.unicod = '%s' and p.prgano = '%d' AND prgstatus = 'A' ORDER BY 2", UNIDADE_IPHAN, $_SESSION['exercicio'] );
				$db->monta_combo("prgcod",$sql,$permissao_formulario,'&nbsp','listar_acoes','','','570','S','prgcod');
				//echo  obrigatorio();
			?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Ação:</td>
			<td><?php

				$sql = array();
				if ( usuario_possui_perfil( PERFIL_COORDENADOR_ACAO_SIGPLAN ) )
				{
					$sql = "
                    SELECT
                        ur.acacod
                    FROM sisplan.usuarioresponsabilidade ur
                    WHERE ur.usucpf = '". $_SESSION['usucpf'] . "'
                      AND ur.acacod is not null
                      AND ur.rpustatus = 'A'";

                    $acacodArr = (array) $db->carregarColuna($sql);
                    if ($acacodArr[0]){
                        $acacodWhere = " AND acacod IN ('" . ( implode("','", $acacodArr) ) . "')";
                        $sql = sprintf( "SELECT distinct acacod as codigo, acacod || ' - ' || acadsc AS descricao FROM monitora.acao WHERE acasnrap = false AND prgano = '%d' AND acasnfinalistica IS TRUE AND acacod IN ('" . ( implode("','", $acacodArr) ) . "') ORDER BY 2", $_SESSION['exercicio'] );
                    }
				}
				if ( $db->testa_superuser() ||
					 usuario_possui_perfil( PERFIL_APOIO_COORDENADOR_PLANEJAMENTO ) ||
					 usuario_possui_perfil( PERFIL_COORDENADOR_PLANEJAMENTO ) ||
					 usuario_possui_perfil( PERFIL_APOIO_COORDENADOR_ORCAMENTO ) ||
					 usuario_possui_perfil( PERFIL_TECNICO_DEPARTAMENTO ) ||
					 usuario_possui_perfil( PERFIL_TECNICO_UNIDADE_DESCENTRALIZADA ) ||
					 usuario_possui_perfil( PERFIL_GESTOR_UNIDADE_DESCENTRALIZADA ) ||
					 usuario_possui_perfil( PERFIL_COORDENADOR_ORCAMENTO ) ) {
					$sql = sprintf( "SELECT distinct acacod as codigo, acacod || ' - ' || acadsc AS descricao FROM monitora.acao a WHERE acasnfinalistica IS TRUE AND unicod = '%s' and acasnrap = false AND prgano = '%d' ORDER BY 2", UNIDADE_IPHAN, $_SESSION['exercicio'] );
				}
				$db->monta_combo("acacod",$sql,$permissao_formulario,'&nbsp','listar_unidades','','','570','S','acacod');
				//echo  obrigatorio();
			?><span id="span_acacod"></span></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Unidade Orçamentária:</td>
			<td><?php

				if ( $acacod )
					$sql = sprintf( "select distinct u.unicod as codigo, u.unicod || ' - ' || u.unidsc as descricao from public.unidade u join monitora.acao a on a.unicod = u.unicod join monitora.programa p on a.prgano = p.prgano and p.prgano = '%d' and a.acacod = '%s' order by 2", $_SESSION['exercicio'], $acacod );
				else
					$sql = sprintf( "select distinct u.unicod as codigo, u.unicod || ' - ' || u.unidsc as descricao from public.unidade u join monitora.acao a on a.unicod = u.unicod join monitora.programa p on a.prgano = p.prgano and p.prgano = '%d' order by 2", $_SESSION['exercicio'] );
				$db->monta_combo("unicod",$sql,$permissao_formulario,'&nbsp','listar_localizadores','','','570','S','unicod');
				//echo  obrigatorio();
			?><span id="span_unicod"></span></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Localizador:</td>
			<td><?php
					if ( $prgcod && $acacod && $unicod )
					$sql = sprintf( "SELECT DISTINCT a.loccod AS codigo, l.loccod || ' - ' || l.locdsc AS descricao FROM public.localizador l JOIN monitora.acao a ON a.loccod = l.loccod WHERE a.prgano = '%d' AND a.unicod = '%s' AND a.prgcod = '%s' AND a.acacod = '%s' ORDER BY 2", $_SESSION['exercicio'], $unicod, $prgcod, $acacod );
				else
					$sql = "SELECT DISTINCT l.loccod AS codigo, l.loccod || ' - ' || l.locdsc AS descricao FROM public.localizador l JOIN monitora.acao a ON a.loccod = l.loccod ORDER BY 2";
//					$db->monta_combo("loccod",$sql,$permissao_formulario,'&nbsp','listar_produto','','','580','S','loccod');
					$db->monta_combo("loccod",$sql,$permissao_formulario,'&nbsp','verificaPo','','','570','S','loccod');
			?><span id="span_loccod"></span></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Plano Orçamentário:</td>
			<td id="tdPlanoOrcamentario">
				<?php
                    $modelo = new PlanoOrcamentario();

					if($prgcod && $acacod && $unicod && $loccod){
						$dados  = $modelo->recuperarTodos("ploid AS codigo, plocod || ' - ' || plonome AS descricao", array("plostatus = 'A'"), "descricao");
						$permissao_formulario_po = 'S';
					}else{
						$dados = array(  array('codigo' => 0, 'descricao' => '') );
						$permissao_formulario_po = 'N';
					}

					$permissao_formulario_po = $permissao_formulario == 'S' ? $permissao_formulario_po : 'N';

                    $db->monta_combo("ploid", $dados, $permissao_formulario_po, '&nbsp', '', '', '', '570', 'S', 'ploid');
                ?>
            </td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Produto Secundário:</td>
			<td>
				<table width="570" border="0" cellpadding="0" cellspacing="2">
					<tr>
						<td width="230" class="SubTituloCentro" style="background-color: #DCDCDC">Produto</td>
						<td width="220" class="SubTituloCentro" style="background-color: #DCDCDC">Unidade Medida</td>
						<td width="120" class="SubTituloCentro" style="background-color: #DCDCDC">Meta</td>
					</tr>
					<tr>
						<td style="text-align:center"><?php
							$sql = "SELECT pduid AS codigo, pdudsc AS descricao FROM planointerno.produto WHERE pdustatus = 'A'";
							$db->monta_combo( "pduid",$sql,$permissao_formulario,"&nbsp;",'','', '', '', 'S', "pduid" );
						?></td>
						<td style="text-align:center"><?php
							$sql = "SELECT udaid AS codigo, udadsc AS descricao FROM planointerno.unidademedida WHERE udastatus = 'A'";
							$db->monta_combo( "udaid",$sql,$permissao_formulario,"&nbsp;",'','', '', '', 'S', "udaid" );
						?></td>
						<td style="text-align:center"><?= campo_texto('pplprosecundariometa','S',$permissao_formulario,'',12,10,'###.###.###','','','Informe a quantidade a ser executada.','','id="pplprosecundariometa"'); ?></td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td colspan="7" class="SubTituloCentro" style="background-color: #DCDCDC">Valor Previsto (anual):</td>
		</tr>
		<?php
			//$style = ($pmuid && $eqdsigla != 'N') ? '' : 'display:none;';
			$style = '';
		?>
        <tr id="trCusteioCapital" style="<?= $style; ?>">
            <td align='right' class="SubTituloDireita"">
                 &nbsp;
            </td>
            <td colspan="5">
                <input type="hidden" id="pmuid" name="pmuid" value="<?= $pmuid; ?>"/>
                <input type="hidden" id="saldoAutorizadoCusteio" name="saldoAutorizadoCusteio" value="<?= $saldoAutorizadoCusteio; ?>" />
                <input type="hidden" id="saldoAutorizadoCapital" name="saldoAutorizadoCapital" value="<?= $saldoAutorizadoCapital; ?>" />
                <input type="hidden" id="saldoDisponivelCusteio" name="saldoDisponivelCusteio" value="<?= $saldoDisponivelCusteio; ?>" />
                <input type="hidden" id="saldoDisponivelCapital" name="saldoDisponivelCapital" value="<?= $saldoDisponivelCapital; ?>" />
                <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 570px; float:left;">
                    <tr>
                        <td class="SubTituloDireita" style="width:510px; text-align: center;">
                            <b>Custeio</b>
                        </td>
                        <td class="SubTituloDireita" style="width:510px; text-align: center;">
                            <b>Capital</b>
                        </td>
                        <td class="SubTituloDireita" style="width:510px; text-align: center;">
                            <b>Total</b>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <?= campo_texto('pplvlrcusteio', 'S', $permissao_formulario, '', 22, 22, '#.###.###.###.###,##', '','','','','id="pplvlrcusteio"','calculaTotalPrevisto()','',''); ?>
                        </td>
                        <td>
                            <?= campo_texto('pplvlrcapital', 'S', $permissao_formulario, '', 22, 22, '#.###.###.###.###,##', '','','','','id="pplvlrcapital"','calculaTotalPrevisto()','',''); ?>
                        </td>
                        <td>
                            <?php
                            	$total = ( str_replace(array('.', ','), array('', '.'), $pplvlrcusteio) ) + ( str_replace(array('.', ','), array('', '.'), $pplvlrcapital) );
                            	echo campo_texto('vlrtotal', 'N', 'N', '', 22, 22, '#.###.###.###.###,##', '','','','','id="vlrtotal"','',formata_valor( $total ),'');
                            ?>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>

		<tr>
			<td colspan="7" class="SubTituloCentro" style="background-color: #DCDCDC">Contratações:</td>
		</tr>
		<tr id="trcontratacoes">
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Contratações:</td>
			<td>
				<table width="80%" align="rigth" border="0" cellspacing="0" cellpadding="2" class="listagem">
					<thead>
						<tr>
							<td width="25%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Forma da contratação</strong></td>
							<td width="32%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Objeto contratado</strong></td>
							<td width="20%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor Custeio</strong></td>
							<td width="20%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor Capital</strong></td>
							<td width="3%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
						</tr>
					</thead>
						<tr bgcolor="#f7f7f7">
							<td width="20%">
								<div id="conitrid_on">
									<? $sql = "SELECT itrid AS codigo, itrdsc AS descricao FROM sisplan.instrumento ORDER BY 2"; ?>
									<? $db->monta_combo("conitrid",$sql,$permissao_formulario,'&nbsp','','','',200,'S','conitrid'); ?>
								</div>
								<div id="conitrid_off"></div>
							</td>
							<td>
								<div id="conobjeto_on" >
									<?= campo_texto('conobjeto', 'S', $permissao_formulario, '', 30, 200, '', '','','','','id="conobjeto"','','','');  ?>
								</div>
								<div id="conobjeto_off"></div>
							</td>
							<td align="right">
								<div id="convlrcusteio_on" >
									<?= campo_texto( "convlrcusteio", 'S', $permissao_formulario, '', 15, 15, '###.###.###.###,##', '', 'left', '', 0, "id='convlrcusteio'"); ?>
								</div>
								<div id="convlrcusteio_off"></div>
							</td>
							<td align="right">
								<div id="convlrcapital_on" >
									<?= campo_texto( "convlrcapital", 'S', $permissao_formulario, '', 15, 15, '###.###.###.###,##', '', 'left', '', 0, "id='convlrcapital'"); ?>
								</div>
								<div id="convlrcapital_off"></div>
							</td>
							<td align="left">
								<a onclick="item_contratacoes_adicionar_do_formulario()" title="Incluir" style="margin: 0; padding: 0 6px; cursor:pointer;">
									<img src="../imagens/gif_inclui.gif" border="0" align="absmiddle"/>
								</a>
							</td>
						</tr>
					<tr>
						<td colspan="5">
							<div class="div_rolagem" id="div_tabela_contratacoes" style="width: 100%; padding: 0; margin: 0; height: 100px;position-y: absolute;">
								<table id="tabela_contratacoes" width="100%" align="left" border="0" cellspacing="0" cellpadding="2">
									<tr valign="bottom">
										<td width="20%" style="height: 0; margin: 0; padding: 0;"></td>
										<td width="37%" style="height: 0; margin: 0; padding: 0;"></td>
										<td width="20%" style="height: 0; margin: 0; padding: 0;"></td>
										<td width="20%" style="height: 0; margin: 0; padding: 0;"></td>
										<td width="3%" style="height: 0; margin: 0; padding: 0;"></td>
									</tr>
								</table>
							</div>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td colspan="7" class="SubTituloCentro" style="background-color: #DCDCDC">Cronogramas:</td>
		</tr>
        <?php
        	//$style = $eqdsigla ? '' : 'display:none;';
        	$style = '';
        ?>
        <tr id="trCronograma" style="<?= $style; ?>">
            <td align='right' class="SubTituloDireita">
               &nbsp;
            </td>
            <td colspan="5">
                    <table cellspacing="0" cellpadding="0" >
                        <tr>
                            <td>
                                <?php
                                	//$style = $eqdsigla == 'F' || $eqdsigla == 'N' ? '' : 'display:none;';
                                	$style = '';
                               	?>
                                <table id="tableFisico" align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 280px; float:left; margin-bottom: 10px; <?= $style; ?>">
                                    <tr>
                                        <td class="SubTituloDireita" style="text-align: center; font-weight: bold;" colspan="3">
                                            Cronograma Físico
                                        </td>
                                    </tr>
                                    <tr>
                                        <td class="SubTituloDireita" style="text-align: center;">
                                            &nbsp;
                                        </td>
                                        <td class="SubTituloDireita" style="text-align: center;">
                                            Previsto
                                        </td>
                                    </tr>
                                    <?php foreach ($arMes as $numMes => $nomeMes) :?>
                                    <tr>
                                        <td align='right' class="SubTituloDireita">
                                            <?= "{$nomeMes}/{$_SESSION['exercicio']}"; ?>
                                        </td>
                                        <td>
                                            <?= campo_texto("crfprevistomes{$numMes}", 'N', $permissao_formulario, '', 22, 22, '#############', '','','','',"id='crfprevistomes{$numMes}'",'','',''); ?>
                                        </td>
                                    </tr>
                                    <?php endforeach;?>
                                </table>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <?php
                                	//$style = $eqdsigla == 'F' || $eqdsigla == 'M' ? '' : 'display:none;';
                                	$style = '';
                                ?>
                                <table id="tableOrcamentario" align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 520px; float: left; margin-bottom: 10px; <?= $style; ?>">
                                    <tr>
                                        <td class="SubTituloDireita" style="width:100px; text-align: center; font-weight: bold;" colspan="3">
                                            Cronograma Orçamentário
                                        </td>
                                    </tr>
                                    <tr>
                                        <td class="SubTituloDireita" style="width:100px; text-align: center;">
                                            &nbsp;
                                        </td>
                                        <td class="SubTituloDireita" style="width:100px; text-align: center;">
                                            Previsto Custeio
                                        </td>
                                        <td class="SubTituloDireita" style="width:100px; text-align: center;">
                                            Previsto Capital
                                        </td>
                                    </tr>
                                    <?php foreach ($arMes as $numMes => $nomeMes) :?>
                                    <tr>
                                        <td align='right' class="SubTituloDireita">
                                            <?= "{$nomeMes}/{$_SESSION['exercicio']}"; ?>
                                        </td>
                                        <td>
                                            <?= campo_texto("croprevistocusteiomes{$numMes}", 'N', $permissao_formulario, '', 22, 22, '#.###.###.###.###,##', '','','','',"id='croprevistocusteiomes{$numMes}'",'','',''); ?>
                                        </td>
                                        <td>
                                            <?= campo_texto("croprevistocapitalmes{$numMes}", 'N', $permissao_formulario, '', 22, 22, '#.###.###.###.###,##', '','','','',"id='croprevistocapitalmes{$numMes}'",'','',''); ?>
                                        </td>
                                    </tr>
                                    <?php endforeach;?>
                                </table>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <?php
                                	//$style = $eqdsigla == 'F' || $eqdsigla == 'M' ? '' : 'display:none;';
                                	$style = '';
                                ?>
                                <table id="tableFinanceiro" align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 520px; float: left; margin-bottom: 10px; <?= $style; ?>">
                                    <tr>
                                        <td class="SubTituloDireita" style="width:100px; text-align: center; font-weight: bold;" colspan="3">
                                            Cronograma Financeiro
                                        </td>
                                    </tr>
                                    <tr>
                                        <td class="SubTituloDireita" style="width:100px; text-align: center;">
                                            &nbsp;
                                        </td>
                                        <td class="SubTituloDireita" style="width:100px; text-align: center;">
                                            Previsto Custeio
                                        </td>
                                        <td class="SubTituloDireita" style="width:100px; text-align: center;">
                                            Previsto Capital
                                        </td>
                                    </tr>
                                    <?php foreach ($arMes as $numMes => $nomeMes) :?>
                                    <tr>
                                        <td align='right' class="SubTituloDireita">
                                            <?= "{$nomeMes}/{$_SESSION['exercicio']}"; ?>
                                        </td>
                                        <td>
                                            <?= campo_texto("cfiprevistocusteiomes{$numMes}", 'N', $permissao_formulario, '', 22, 22, '#.###.###.###.###,##', '','','','',"id='cfiprevistocusteiomes{$numMes}'",'','',''); ?>
                                        </td>
                                        <td>
                                            <?= campo_texto("cfiprevistocapitalmes{$numMes}", 'N', $permissao_formulario, '', 22, 22, '#.###.###.###.###,##', '','','','',"id='cfiprevistocapitalmes{$numMes}'",'','',''); ?>
                                        </td>
                                    </tr>
                                    <?php endforeach;?>
                                </table>
                            </td>
                        </tr>
                    </table>
            </td>
        </tr>
        <tr style="background-color: #cccccc">
            <td align="right" style="width:25%;">
                &nbsp;
            </td>
            <td colspan="7">
                <?php if($permissao_formulario == 'S') :?>
                    <input type="button" name="btnGravar" value="Gravar" onclick="enviar();"/>
                <?php endif;?>
                <input type="button" name="btnVoltar" value="Cancelar" onclick="cancelar();"/>
            </td>
        </tr>
    </table>
</form>


<script type="text/javascript">

	var p_prgcod = '<?= $prgcod; ?>';
	var p_acacod = '<?= $acacod; ?>';
	var p_unicod = '<?= $unicod; ?>';
	var p_loccod = '<?= $loccod; ?>';
	var p_iteid = '<?= $iteid; ?>';
	var p_sbcid = '<?= $sbcid; ?>';

	jQuery(document).ready(function() {

		if (jQuery('#esfid').val() == '' ){
			document.getElementById('atisndetalharregionalizacao').disabled  = true;
		}else{
			action_esfera(jQuery('#esfid').val());
		}
		if (jQuery('#estuf').val() != '' ){
			listar_municipios(jQuery('#estuf').val(),jQuery('#hdn_muncod').val());
		}

	});

	function cancelar() {
	    window.location.href = '?modulo=principal/consultarPlanejamento&acao=A';
	}


    function enviar( acao ){

        var d = document;
        var f = d.formulario;
        var msg = '';

        if (f.uexid.value == ''){
            msg += '\n\tUnidade Executora';
        }

        if (f.tppid.value == ''){
            msg += '\n\tTipo de Projeto';
        }

        if (f.pplprioridade.value == ''){
            msg += '\n\tPrioridade';
        }

        if (f.ppltitulo.value == ''){
            msg += '\n\tTítulo';
        }

        if (f.pplobjetivo.value == ''){
            msg += '\n\tObjetivo';
        }

        if (f.ppljustificativa.value == ''){
            msg += '\n\tJustificativa';
        }

        if (f.ppldescricao.value == ''){
            msg += '\n\tDescrição das Atividades';
        }

        selectAllOptions( document.getElementById('vinculacaoPlanejamento') );
		if( !jQuery('#vinculacaoPlanejamento option').val() != '' ){
            msg += '\n\tVinculação ao Planejamento Estratégico';
		}

		if (f.esfid.value == ''){
            msg += '\n\tLocalização da ação';
        }

        if (f.prgcod.value == ''){
            msg += '\n\tPrograma';
        }

        if (f.acacod.value == ''){
            msg += '\n\tAção';
        }

        if (f.unicod.value == ''){
            msg += '\n\tUnidade Orçamentária';
        }

        if (f.loccod.value == ''){
            msg += '\n\tLocalizador';
        }

        if (f.ploid.value == ''){
            msg += '\n\tPlano Orçamentário';
        }

        if (f.pduid.value == ''){
            msg += '\n\tProduto';
        }

        if (f.udaid.value == ''){
            msg += '\n\tUnidade Medida';
        }

        if (f.pplprosecundariometa.value == ''){
            msg += '\n\tMeta';
        }

        if (f.pplvlrcusteio.value == ''){
            msg += '\n\tValor Previsto (anual) de Custeio';
        }

        if (f.pplvlrcapital.value == ''){
            msg += '\n\tValor Previsto (anual) de Captal';
        }

        var valorTotalCusteio    = 0;
        var valorTotalCapital    = 0;
        var valorTotalCusteio2   = 0;
        var valorTotalCapital2   = 0;

        var valorPrevistoCusteio = parseFloat(jQuery('#pplvlrcusteio').val().split('.').join('').split(',').join('.'));
        var valorPrevistoCapital = parseFloat(jQuery('#pplvlrcapital').val().split('.').join('').split(',').join('.'));

        jQuery('[id^=croprevistocusteiomes]').each(function(i, elem){
            var valor = parseFloat(elem.value.split('.').join('').split(',').join('.'));
            if(valor){
            	valorTotalCusteio += valor;
            }
        });

        jQuery('[id^=cfiprevistocusteiomes]').each(function(i, elem){
            var valor = parseFloat(elem.value.split('.').join('').split(',').join('.'));
            if(valor){
            	valorTotalCusteio2 += valor;
            }
        });



        jQuery('[id^=croprevistocapitalmes]').each(function(i, elem){
            var valor = parseFloat(elem.value.split('.').join('').split(',').join('.'));
            if(valor){
            	   valorTotalCapital += parseFloat(elem.value.split('.').join('').split(',').join('.'));
            }
        });

        jQuery('[id^=cfiprevistocapitalmes]').each(function(i, elem){
            var valor = parseFloat(elem.value.split('.').join('').split(',').join('.'));
            if(valor){
            	   valorTotalCapital2 += parseFloat(elem.value.split('.').join('').split(',').join('.'));
            }
        });

        valorTotalCusteio  = valorTotalCusteio.toFixed(2);
        valorTotalCapital  = valorTotalCapital.toFixed(2);
        valorTotalCusteio2 = valorTotalCusteio2.toFixed(2);
        valorTotalCapital2 = valorTotalCapital2.toFixed(2);

        if(valorTotalCusteio  != valorPrevistoCusteio || valorTotalCapital  != valorPrevistoCapital
        || valorTotalCusteio2 != valorPrevistoCusteio || valorTotalCapital2 != valorPrevistoCapital){
            msg += '\n\tA soma dos valores lançados nos cronogramas Orçamentário e Financeiro deve ser igual ao Valor Previsto (anual).';
        }

        var valorTotalCusteio = 0;
        var valorTotalCapital = 0;
        var pplvlrcusteio = parseFloat(jQuery('#pplvlrcusteio').val().split('.').join('').split(',').join('.'));
        var pplvlrcapital = parseFloat(jQuery('#pplvlrcapital').val().split('.').join('').split(',').join('.'));

        jQuery('[name^=valorcusteio]').each(function(i, elem){
            var valor = parseFloat(elem.value.split('.').join('').split(',').join('.'));
            if(valor){
            	valorTotalCusteio += parseFloat(elem.value.split('.').join('').split(',').join('.'));
            }
        });

        jQuery('[name^=valorcapital]').each(function(i, elem){
            var valor = parseFloat(elem.value.split('.').join('').split(',').join('.'));
            if(valor){
            	valorTotalCapital += parseFloat(elem.value.split('.').join('').split(',').join('.'));
            }
        });

        valorTotalCusteio = valorTotalCusteio.toFixed(2);
        valorTotalCapital = valorTotalCapital.toFixed(2);

        if (valorTotalCusteio != pplvlrcusteio){
            msg += '\n\tA soma dos valores de custeio das contratações deve ser igual ao Valor Previsto (anual).';
        }

        if (valorTotalCapital != pplvlrcapital){
            msg += '\n\tA soma dos valores de capital das contratações deve ser igual ao Valor Previsto (anual).';
        }

        if (msg != ''){
            alert('Os campos listados são obrigatórios e devem ser preenchidos:' + msg);
            return false;
        }


		// Verificando a regra de Prioridade, o sistema fará um ajax para verificar se já existe
		// Algum registro na mesma unidade
		var prioridade	= jQuery('#pplprioridade').val();
		var unidade		= jQuery('#uexid').val();
		var pplid		= jQuery('#pplid').val();

		jQuery.ajax({
			  url: "?modulo=principal/incluirPlanejamento&acao=A&act=validaPrioridade&pplid="+pplid+"&prioridade="+prioridade+"&unidade="+unidade,
			  type: "GET",
			  success: function( retorno ){
			    if ( retorno > 0 ){
					var r = confirm("Existe uma solicitação com a prioridade informada, deseja substituir e atualizar a prioridade do projeto anterior?");
					if ( r == true ){

						jQuery.ajax({
							  url: "?modulo=principal/incluirPlanejamento&acao=A&act=atualizaPrioridades&pplid="+pplid+"&prioridade="+prioridade+"&unidade="+unidade,
							  type: "GET",
							  success:function( retorno ){
							  	if( retorno == 'true' ){
							        selectAllOptions( f.vinculacaoPlanejamento );
									document.formulario.submit();
							  	}
							  }
						});


					}
			    }else{
			        selectAllOptions( f.vinculacaoPlanejamento );
					document.formulario.submit();
			    }
			  }
		});

    }

    function calculaTotalPrevisto(){
    	var custeio = parseFloat(jQuery('#pplvlrcusteio').val().split('.').join('').split(',').join('.'));
    	var capital = parseFloat(jQuery('#pplvlrcapital').val().split('.').join('').split(',').join('.'));

    	if ( !custeio ){
    		custeio = 0;
    	}

    	if ( !capital ){
    		capital = 0;
    	}

		var total	= custeio + capital;
		jQuery('#vlrtotal').val( MascaraMonetario(new String(total)) );
    }

    //JAVASCRIPT DE REGIONALIZAÇÃO
    function action_esfera(esfid){

		if ( document.formulario.esfid.value == '' )
			document.getElementById('atisndetalharregionalizacao').disabled  = true;
		else
			document.getElementById('atisndetalharregionalizacao').disabled  = false;



		if (document.formulario.esfid.value == 4)
		{
			document.getElementById('abrestuf_on').style.display = 'none';
			document.getElementById('abrestuf_off').style.display = '';
			document.getElementById('abrmuncod_on').style.display = 'none';
			document.getElementById('abrmuncod_off').style.display = '';
		}
		else
		{
			document.getElementById('abrestuf_on').style.display = '';
			document.getElementById('abrestuf_off').style.display = 'none';
			document.getElementById('abrmuncod_on').style.display = '';
			document.getElementById('abrmuncod_off').style.display = 'none';
		}

		if( document.getElementById('atisndetalharregionalizacao').checked == false ){
			document.getElementById('trabrangencia').style.display = 'none';
		}else{
			document.getElementById('trabrangencia').style.display = '';
		}

		esfid = esfid ? esfid : document.formulario.esfid.value;

		if(esfid=='')
		{
			document.formulario.paiid.disabled = true;
			document.formulario.paiid.value = 1;
			document.getElementById('estuf_on').style.display='none';
			document.getElementById('estuf_off').style.display='block';
			document.getElementById('muncod_on').style.display='none';
			document.getElementById('muncod_off').style.display='block';

			document.getElementById('estuf').value='';
			document.getElementById('muncod').value='';
		}
		if(esfid==1)
		{
			document.formulario.paiid.disabled = true;
			document.formulario.paiid.value = 1;
		}
		else if(esfid==2)
		{
			document.formulario.paiid.disabled = true;
			document.formulario.paiid.value = 1;
			document.getElementById('estuf').value='<?=$estuf ?>';
			//document.getElementById('muncod').value='';

			listar_municipios( document.getElementById('estuf').value );

			document.getElementById('estuf_on').style.display='block';
			document.getElementById('estuf_off').style.display='none';
			document.getElementById('muncod_on').style.display='block';
			document.getElementById('muncod_off').style.display='none';
		}
		else if(esfid==3)
		{
			document.formulario.paiid.disabled = true;
			document.formulario.paiid.value = 1;
			document.getElementById('estuf').value  = '<?=$estuf ?>';
			document.getElementById('muncod').value = '';

			<?
			if (!$muncod){
			?>
			listar_municipios( document.getElementById('estuf').value );
			<?
			}else{
				if ($estuf == 'DF'):
			?>
			document.getElementById('muncod').value = '<?=$rgaid ?>';
			<?
				else:
			?>
			document.getElementById('muncod').value = '<?=$muncod ?>';
			<?
				endif;
			}
			?>
			document.getElementById('estuf_on').style.display='block';
			document.getElementById('estuf_off').style.display='none';
		}
		else if(esfid==4)
		{
			document.formulario.paiid.disabled = false;
			document.getElementById('estuf_on').style.display='none';
			document.getElementById('estuf_off').style.display='block';
			document.getElementById('muncod_on').style.display='none';
			document.getElementById('muncod_off').style.display='block';

			document.getElementById('estuf').value='';
			document.getElementById('muncod').value='';
		}

	}

	function listar_municipios( regcod, muncod ){

		var campo_select = document.getElementById( 'muncod' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );

		if(document.formulario.esfid.value!='')
		{
			document.getElementById('muncod_on').style.display='block';
			document.getElementById('muncod_off').style.display='none';

			if ( muncod ){
				new Ajax.Request('../ajax/lista_mun_pi.php?estuf='+regcod+'&muncodi='+muncod, { method:'get'} );
			}else{
				new Ajax.Request('../ajax/lista_mun_pi.php?estuf='+regcod, { method:'get'} );
			}
		}
	}

	function exibeAbrangencia()
	{
		document.getElementById('abrpaiid').value = 'Brasil';

		if (document.getElementById('trabrangencia').style.display == 'none' )
		{
			document.getElementById('trabrangencia').style.display = '';
		}
		else
		{
			document.getElementById('trabrangencia').style.display = 'none';
		}
	}


	function listar_municipios_abrangencia( regcod )
	{
		var campo_select = document.getElementById( 'abrmuncod' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );

		ajaxGet('../ajax/lista_mun_abran.php?abrestuf='+regcod,document.getElementById('abrmuncod'),true);
	}

	///////////////////////////////////////////////
	//
	// ITENS CONTRATAÇÕES
	//
	//////////////////////////////////////////////
	var item_contratacoes_id_atual = 0;
	var item_contratacoes_adicionados = {};

	function item_contratacoes_adicionar_do_formulario()
	{
		// captura os campos
		var forma 		= document.getElementById( 'conitrid' );
		var forma_dsc 	= forma.options[forma.selectedIndex].text;
		var objeto		= document.getElementById( 'conobjeto' );
		var valorcusteio		= document.getElementById( 'convlrcusteio' );
		var valorcapital		= document.getElementById( 'convlrcapital' );

		// realiza validação dos dados
		if ( !validaBranco( forma, 'Forma de contratação' ) ) return;
		if ( !validaBranco( objeto,'Objeto contratado' ) ) return;
		if ( !validaBranco( valorcusteio, 'Valor Custeio' ) ) return;
		if ( !validaBranco( valorcapital, 'Valor Capital' ) ) return;

		// insere os dados na tabela
		if ( item_contratacoes_adicionar( forma.value, forma_dsc , objeto.value, valorcusteio.value, valorcapital.value ) == false ){
			return;
		}

		// limpa campos para nova inserção
		forma.value = '';
		objeto.value = '';
		valorcapital.value = '';
		valorcusteio.value = '';
		item_abrangencia_id_atual++;

	}

	function item_contratacoes_adicionar( forma, forma_dsc, objeto, valorcusteio, valorcapital )
	{
		// define dados gerais do item a ser adicionado
		item_contratacoes_id_atual++;
		var tabela = document.getElementById( 'tabela_contratacoes' );
		var id = 'item_contratacoes_' + item_contratacoes_id_atual;

		// cria nova linha para a tabela de financeiro
		var nova_linha = tabela.insertRow( tabela.rows.length - 1 );
		    nova_linha.id = id;
		    nova_linha.onmouseover = new Function( ' this.style.backgroundColor = "#ffffcc"; ' );
		// FIM cria nova linha para a tabela de financeiro

		// cria as célular da nova linha
		var celula_forma = nova_linha.insertCell( 0 );
		var celula_objeto = nova_linha.insertCell( 1 );
		    celula_objeto.style.textAlign = 'right';
		    celula_objeto.style.color = '#0066cc';
		var celula_valorcusteio = nova_linha.insertCell( 2 );
			celula_valorcusteio.style.textAlign = 'right';
			celula_valorcusteio.style.color = '#0066cc';
			var celula_valorcapital = nova_linha.insertCell( 3 );
			celula_valorcapital.style.textAlign = 'right';
			celula_valorcapital.style.color = '#0066cc';
		var celula_acao_alterar = nova_linha.insertCell( 4 );
		    celula_acao_alterar.style.textAlign = 'center';
		var celula_acao_remover = nova_linha.insertCell( 5 );
		    celula_acao_remover.style.textAlign = 'left';
		// FIM cria as célular da nova linha

		// cria campos ocultos com os dados do novo item
		var input_hidden_forma = item_abrangencia_criar_input( forma, 'forma' );
		var input_hidden_objeto = item_abrangencia_criar_input( objeto, 'objeto' );
		var input_hidden_valorcusteio = item_abrangencia_criar_input( valorcusteio, 'valorcusteio' );
		var input_hidden_valorcapital = item_abrangencia_criar_input( valorcapital, 'valorcapital' );
			input_hidden_forma.id = id + '_valor';
		// FIM cria campos ocultos com os dados do novo item
		// define dado para as células de açãoe

			var link_acao_alterar = document.createElement( 'a' );
			    link_acao_alterar.href = 'javascript:item_contratacoes_alterar( \'' + id + '\', \'' + forma + '\', \'' + objeto + '\', \'' + valorcusteio + '\', \'' + valorcapital + '\' );';
			    link_acao_alterar.title = 'Editar';
			    link_acao_alterar.innerHTML = '<img src="../imagens/alterar.gif" border="0" align="absmiddle"/>';
			var link_acao_remover = document.createElement( 'a' );
			    link_acao_remover.href = 'javascript:item_contratacoes_remover( \'' + id + '\', true );';
			    link_acao_remover.title = 'Excluir';
			    link_acao_remover.innerHTML = '<img src="../imagens/excluir.gif" border="0" align="absmiddle"/>';

		link_acao_alterar.id = id + '_ancora';
		// FIM define dado para as células de ação
		// insere os dados ( textos e inputs ) nas células
		celula_forma.appendChild( document.createTextNode( forma_dsc ) );
		celula_forma.appendChild( input_hidden_forma );
		celula_objeto.appendChild( document.createTextNode( objeto ) );
		celula_objeto.appendChild( input_hidden_objeto );

		celula_valorcusteio.appendChild( document.createTextNode( valorcusteio ) );
		celula_valorcusteio.appendChild( input_hidden_valorcusteio );

		celula_valorcapital.appendChild( document.createTextNode( valorcapital ) );
		celula_valorcapital.appendChild( input_hidden_valorcapital );

		if ( link_acao_alterar != null  )
		{
			celula_acao_alterar.appendChild( link_acao_alterar );
		}
		if ( link_acao_remover != null  )
		{
			celula_acao_remover.appendChild( link_acao_remover );
		}

		return true;
	}

	function item_contratacoes_alterar( id_tr, forma, objeto, valorcusteio, valorcapital )
	{

		// captura os campos do financeiro
		var conitrid		= document.getElementById( 'conitrid' );
		var conobjeto		= document.getElementById( 'conobjeto' );
		var convlrcusteio	= document.getElementById( 'convlrcusteio' );
		var convlrcapital	= document.getElementById( 'convlrcapital' );

		conitrid.value			= forma;
		conobjeto.value			= objeto;
		convlrcusteio.value		= valorcusteio;
		convlrcapital.value		= valorcapital;

		item_abrangencia_remover( id_tr, false );
	}

	function item_contratacoes_remover( id_tr, realizar_pergunta ){
		if ( realizar_pergunta ){
			if ( !confirm( 'Deseja realmente excluir a contratação?' ) ){
				return;
			}
		}
		houve_modificacao = true;
		var tr = document.getElementById( id_tr );
		var input = document.getElementById( id_tr + '_valor' );
		if ( tr && input ){
			tr.parentNode.removeChild( tr );
		}
	}

	///////////////////////////////////////////////
	//
	// ITENS ABRNGENCIA
	//
	//////////////////////////////////////////////
	var item_abrangencia_id_atual = 0;

	var item_abrangencia_adicionados = {};

	function item_abrangencia_adicionar_do_formulario()
	{
		// captura os campos
		var pais = document.getElementById( 'abrpaiid' );
		var uf 	 = document.getElementById( 'abrestuf' );
		var mun  = document.getElementById( 'abrmuncod' );

		// realiza validação dos dados
		if ( !validaBranco( pais, 'País' ) ) return;
		if( document.formulario.esfid.value != 4 )
		{
			if ( !validaBranco( uf, 'Unidade Federativa' ) ) return;
			if ( !validaBranco( mun, 'Município' ) ) return;
		}


		// insere os dados na tabela
		if ( item_abrangencia_adicionar( pais.value, uf.value, mun.value ) == false )
		{
			return;
		}
		// limpa campos para nova inserção
		pais.value = '';
		uf.value = '';
		mun.value = '';
		//item_financeiro_preencher_valores_default();
		houve_modificacao = true;
		document.getElementById( 'abrpaiid' ).focus();
	}

	function item_abrangencia_adicionar( pais, uf, mun )
	{

		var id_novo_item = pais + '_' + uf + '_' + mun;
		// verifica se item já existe
		if ( item_abrangencia_adicionados[id_novo_item] )
		{
			alert( 'Já existe uma abrangência com essas informações. Você pode alterar as informações selecionando o item correspondente na tabela abaixo do formulário.' );
			// posiciona a linha a ser destacada
			document.getElementById( item_abrangencia_adicionados[id_novo_item] + '_ancora' ).focus();
			var funcao_hilight = new Function( ' item_abrangencia_hilight( \'' + item_abrangencia_adicionados[id_novo_item] + '\', 30 ); ' );
			setTimeout( funcao_hilight, 300 );
			return false;

		}

		// define dados gerais do item a ser adicionado
		item_abrangencia_id_atual++;
		var tabela = document.getElementById( 'tabela_abrangencia' );
		var id = 'item_abragencia_' + item_abrangencia_id_atual;
		item_abrangencia_adicionados[id_novo_item] = id;

		// FIM define dados gerais do item a ser adicionado
		// cria nova linha para a tabela de financeiro
		var nova_linha = tabela.insertRow( tabela.rows.length - 1 );
		    nova_linha.id = id;
		    nova_linha.onmouseover = new Function( ' this.style.backgroundColor = "#ffffcc"; ' );
		// FIM cria nova linha para a tabela de financeiro
		// cria as célular da nova linha
		var celula_pais = nova_linha.insertCell( 0 );
		var celula_uf = nova_linha.insertCell( 1 );
		    celula_uf.style.textAlign = 'right';
		    celula_uf.style.color = '#0066cc';
		var celula_mun = nova_linha.insertCell( 2 );
		    celula_mun.style.textAlign = 'right';
		    celula_mun.style.color = '#0066cc';
		var celula_acao_alterar = nova_linha.insertCell( 3 );
		    celula_acao_alterar.style.textAlign = 'center';
		var celula_acao_remover = nova_linha.insertCell( 4 );
		    celula_acao_remover.style.textAlign = 'left';
		// FIM cria as célular da nova linha
		// cria campos ocultos com os dados do novo item
		var input_hidden_pais = item_abrangencia_criar_input( pais, 'pais' );
		var input_hidden_uf = item_abrangencia_criar_input( uf, 'uf' );
		var input_hidden_mun = item_abrangencia_criar_input( mun, 'mun' );
			input_hidden_pais.id = id + '_valor';
		// FIM cria campos ocultos com os dados do novo item
		// define dado para as células de açãoe

			var link_acao_alterar = document.createElement( 'a' );
			    link_acao_alterar.href = 'javascript:item_abrangencia_alterar( \'' + id + '\', \'' + pais + '\', \'' + uf + '\', \'' + mun + '\' );';
			    link_acao_alterar.title = 'Editar';
			    link_acao_alterar.innerHTML = '<img src="../imagens/alterar.gif" border="0" align="absmiddle"/>';
			var link_acao_remover = document.createElement( 'a' );
			    link_acao_remover.href = 'javascript:item_abrangencia_remover( \'' + id + '\', true );';
			    link_acao_remover.title = 'Excluir';
			    link_acao_remover.innerHTML = '<img src="../imagens/excluir.gif" border="0" align="absmiddle"/>';

		link_acao_alterar.id = id + '_ancora';
		// FIM define dado para as células de ação
		// insere os dados ( textos e inputs ) nas células
		celula_pais.appendChild( document.createTextNode( pais ) );
		celula_pais.appendChild( input_hidden_pais );
		celula_uf.appendChild( document.createTextNode( uf ) );
		celula_uf.appendChild( input_hidden_uf );
		celula_mun.appendChild( document.createTextNode( mun ) );
		celula_mun.appendChild( input_hidden_mun );

		if ( link_acao_alterar != null  )
		{
			celula_acao_alterar.appendChild( link_acao_alterar );
		}
		if ( link_acao_remover != null  )
		{
			celula_acao_remover.appendChild( link_acao_remover );
		}


		listar_municipios_abrangencia();

		return true;
	}

	function item_abrangencia_criar_input( valor, nome_campo )
	{
		var input = document.createElement( 'input' );
		input.type = 'hidden';
		input.name = nome_campo + '[]';
		input.value = valor;
		return input;
	}

	function item_abrangencia_alterar( id_tr, pais, uf, mun )
	{
		houve_modificacao = true;
		// captura os campos do financeiro
		var campo_pais = document.getElementById( 'abrpaiid' );
		var campo_uf = document.getElementById( 'abrestuf' );
		var campo_mun = document.getElementById( 'abrmuncod' );
		listar_municipios_abrangencia(uf);
		campo_pais.value = pais;
		campo_uf.value = uf;
		campo_mun.value = mun;
		item_abrangencia_remover( id_tr, false );
	}

	function item_abrangencia_remover( id_tr, realizar_pergunta )
	{
		if ( realizar_pergunta )
		{
			if ( !confirm( 'Deseja realmente excluir o detalhamento?' ) )
			{
				return;
			}
		}
		houve_modificacao = true;
		var tr = document.getElementById( id_tr );
		var input = document.getElementById( id_tr + '_valor' );
		if ( tr && input )
		{
			tr.parentNode.removeChild( tr );
		}
	}

	function item_abrangencia_hilight( id_tr, loop )
	{
		var tr = document.getElementById( id_tr );
		if ( !tr )
		{
			return;
		}
		if ( loop < 1 )
		{
			return;
		}
		var cor = '';
		switch ( loop-- % 9 )
		{
			case 0:	case 8:
				//cor = '#d0d0d0';
				cor = '#ffffff';
				break;
			case 1:	case 7:
				//cor = '#d8d8d8';
				cor = '#ffffdd';
				break;
			case 2:	case 6:
				//cor = '#e0e0e0';
				cor = '#ffffaa';
				break;
			case 3:	case 5:
				//cor = '#e8e8e8';
				cor = '#ffff80';
				break;
			case 4:
				//cor = '#f0f0f0';
				cor = '#ffff40';
				break;
		}
		tr.style.backgroundColor = cor;
		var funcao_hilight = new Function( ' item_abrangencia_hilight( \'' + id_tr + '\', ' + loop + ' ); ' );
		setTimeout( funcao_hilight, 120 );
	}

	function listar_ptres(id)
	{
		if ( id != '' )
			ajaxGet('../ajax/lista_campos_ptres.php?ptres='+id,document.getElementById('span_ptres'),true);

		verificaPo();
	}

	function listar_acoes( prgcod )
	{
		var campo_select = document.getElementById( 'acacod' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );

		verificaPo();

		if( prgcod != '' )
		{
			ajaxGet('../ajax/lista_acoes.php?prgcod='+prgcod+'&acacod='+p_acacod,document.getElementById('span_acacod'),true);
		}
	}

	function listar_unidades( acacod )
	{
		var campo_select = document.getElementById( 'unicod' );
		var campo_dep = document.getElementById( 'depid' );

		if( campo_select ){
			while( campo_select.options.length ){
				campo_select.options[0] = null;
			}
		}

		if ( campo_dep ){
			while( campo_dep.options.length ){
				campo_dep.options[0] = null;
			}
		}

		campo_select.options[0] = new Option( '', '', false, true );

		verificaPo();

		if( acacod != '' )
		{
			ajaxGet('../ajax/lista_unidades.php?acacod='+acacod+'&unicod='+p_unicod,document.getElementById('span_unicod'),true);
			//listagem_produto( acacod );
			//listar_coordenador( acacod );
		}
	}

	function listagem_produto( acacod )
	{
		var prgcod = document.formulario.prgcod.value && document.formulario.prgcod.value != 'null' ? document.formulario.prgcod.value : p_prgcod;
		var acacod = document.formulario.acacod.value && document.formulario.acacod.value != 'null' ? document.formulario.acacod.value : p_acacod;
		var unicod = document.formulario.unicod.value && document.formulario.unicod.value != 'null' ? document.formulario.unicod.value : p_unicod;

		var campo_select = document.getElementById( 'prodsc' );
		//while( campo_select.options.length )
		//{
		//	campo_select.options[0] = null;
		//}
		//campo_select.options[0] = new Option( '', '', false, true );

		verificaPo();

		if( acacod != '' )
		{
			ajaxGet('../ajax/lista_produto.php?prgcod='+prgcod+'&acacod='+acacod+'&unicod='+unicod+'&loccod=',true);
			listar_departamentos( acacod );
		}
	}

	function listar_localizadores( unicod )
	{
		var prgcod = document.formulario.prgcod.value && document.formulario.prgcod.value != 'null' ? document.formulario.prgcod.value : p_prgcod;
		var acacod = document.formulario.acacod.value && document.formulario.acacod.value != 'null' ? document.formulario.acacod.value : p_acacod;
		var unicod = document.formulario.unicod.value && document.formulario.unicod.value != 'null' ? document.formulario.unicod.value : p_unicod;

		var campo_select = document.getElementById( 'loccod' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );

		verificaPo();

		if( unicod !='' && prgcod != '' && acacod != '')
		{
			ajaxGet('../ajax/lista_localizadores.php?prgcod='+prgcod+'&acacod='+acacod+'&unicod='+unicod+'&loccod='+p_loccod,document.getElementById('span_loccod'),true);
		}
	}

	function listar_produto( loccod )
	{
		var prgcod = document.formulario.prgcod.value && document.formulario.prgcod.value != 'null' ? document.formulario.prgcod.value : p_prgcod;
		var acacod = document.formulario.acacod.value && document.formulario.acacod.value != 'null' ? document.formulario.acacod.value : p_acacod;
		var unicod = document.formulario.unicod.value && document.formulario.unicod.value != 'null' ? document.formulario.unicod.value : p_unicod;

		if ( loccod != '' && prgcod != '' && acacod != '' && unicod != '' )
		{
			var url = '../ajax/lista_produto.php?prgcod='+prgcod+'&acacod='+acacod+'&unicod='+unicod+'&loccod='+loccod;
			ajaxGet(url, document.getElementById('codprg'),true);
		}
	}

	function verificaPo(){
		var prgcod = jQuery('#prgcod').val();
		var acacod = jQuery('#acacod').val();
		var unicod = jQuery('#unicod').val();
		var loccod = jQuery('#loccod').val();

		if(prgcod && acacod && unicod && loccod){
			jQuery('#tdPlanoOrcamentario').load('?modulo=principal/incluirPlanejamento&acao=A&act=filtraPlanoOrcamentario&prgcod='+prgcod+'&acacod='+acacod+'&unicod='+unicod+'&loccod='+loccod);
			jQuery('#ploid').attr('disabled','');
		}else{
			jQuery('#ploid').val('');
			jQuery('#ploid').attr('disabled','disabled');
		}
	}

	<?php

	  if($pplid){
	  	  //Carregando Abrangência
		  $sql = "
				SELECT
					p.paidescricao AS pais,
					e.estuf||' - '||e.estdescricao AS uf,
					CASE r.estuf
						WHEN 'DF' THEN ra.rgadsc
						ELSE m.estuf||' - '||m.mundescricao
					END AS mun
				FROM
					sisplan.projetoregionalizacao r
					JOIN territorios.pais p
						ON p.paiid = r.paiid
					LEFT JOIN territorios.municipio m
						ON m.muncod = r.muncod
					LEFT JOIN territorios.estado e
						ON e.estuf = r.estuf
					LEFT JOIN planointerno.regiaoadministrativa ra
						ON ra.rgaid = r.rgaid
				WHERE
					r.pplid = ".(int) $pplid;

		    $dados = $db->carregar($sql);
		    $dados = is_array($dados) ? $dados : array();

			if ( !empty($dados) ){
				?>
						document.getElementById('atisndetalharregionalizacao').checked = true;
						document.getElementById('trabrangencia').style.display = '';
				<?php

					foreach ($dados as $dado):
				?>
						item_abrangencia_adicionar('<?=$dado['pais']?>', '<?=$dado['uf']?>', '<?=$dado['mun']?>');
				<?php
					endforeach;
			}

			//Carregando Contratações
            $modelo = new ProjetoContratacao();
			$dados  = $modelo->getListaContratacoesByPplid( (int) $pplid);

			$dados = is_array($dados) ? $dados : array();
			if ( !empty($dados) ){
					foreach ($dados as $dado):
				?>
					    item_contratacoes_adicionar( <?= $dado['forma']; ?>, '<?= $dado['forma_dsc']; ?>', '<?= $dado['objeto']; ?>', '<?= formata_valor( $dado['valorcusteio']); ?>', '<?= formata_valor( $dado['valorcapital']); ?>' )
				<?php
					endforeach;
			}
	  }
	?>
</script>