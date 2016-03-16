<?php
/**
 * Esta função verifica se o usuário informado já foi cadastro no projeto.
 * Se não faz a inclusão, se sim atualiza o mesmo.
 * 
 * @param $pje
 * @param $usu
 * @param $influ
 * @param $inter
 * @return void
 */
function atualiza_ator($pje,$usu,$influ,$inter)
{
	global $db;
	// verifica se o projeto tem visibilidade aberta a todos //
	if ($usu <>'')
	{
		$sql =	' SELECT ' .
					'usucpf' .
				' FROM ' .
					'monitora.projeto_ator' .
				' WHERE ' . 
					'usucpf' . ' = ' . '\'' . $usu . '\'' . 
				' AND ' .
					'pjeid' . ' = ' . $pje .
				'';
		if ($db->pegaUm($sql))
		{
			$sql =	' UPDATE ' .
						'monitora.projeto_ator' .
					' SET ' . 
						'peainfluencia' . ' = ' . '\'' . $influ . '\'' .
						',' .
						'peainteresse' . ' = ' . '\'' . $inter . '\'' . 
						',' . 
						'peastatus' . ' = ' . '\'' . 'A' . '\'' . 
					' WHERE ' . 
						'usucpf' . ' = ' . '\'' . $usu . '\'' . 
					' AND ' . 
						'pjeid' . ' = ' . $pje . 
					'';
		}
		else 
		{
			$sql =	' INSERT INTO '.
						'monitora.projeto_ator' .
						' ( ' .
							'pjeid' . 
							',' . 
							'usucpf' . 
							',' . 
							'peainfluencia' . 
							',' . 
							'peainteresse' . 
						' ) ' . 
					' VALUES ' . 
						' ( ' . 
							$pje . 
							',' . 
							'\'' . $usu . '\'' . 
							',' . 
							'\'' . $influ . '\'' . 
							',' . 
							'\'' . $inter . '\'' . 
						' ) ' .
					'';
		}
		$saida = $db->executar($sql);
	}
}

/**
 * Aprova um projeto
 * 
 * @param $pto
 * @return void
 */
function atualiza_aprov($pto)
{
	global $db;
	
	$sql = ' UPDATE ' .
				'monitora.planotrabalho' . 
			' SET '. 
				'ptosnaprovado' . ' = ' . '\''. 't' . '\'' . ' 
			WHERE ' . 
				'ptoid' . ' = ' . $pto;
    $saida = $db->executar($sql);
}

/**
 * Checa se o projeto esta em aberto
 * @return boolean
 */

function projetoaberto()
 {
 	 global $db;
	 
 	// verifica se o projeto esta aberto para ser acompanhado,	//
 	// ou seja, se ele nao esta concluido, cancelado etc.		//
 	
	/*
	"1",Atrasado
	"2",Cancelado
	"3",Concluido
	"4",Em dia
	"5",Nao iniciado
	"6",Paralisado
	"7",Suspenso"
	"8",Sem andamento
	"9",Iniciado"
	10 - fase de planejamento
	*/
	
	$sql = ' SELECT ' .
				'tpscod' .
			' FROM ' .
				'monitora.projetoespecial' .
			' WHERE ' .
				'pjeid' . ' = ' . $_SESSION['pjeid'];
				
	$sit=$db->pegaUm($sql);
	if ($sit=='1' or $sit=='4' or $sit=='9' or $sit=='6' or $sit=='8' or $sit=='10')
	{
		// se o projeto estiver atrasasdo, ou em dia, ou iniciado, ou sem andamento ou paralisado ent	//
		// pode acompanhar																					//
		return true;
	}
//	else
	{
		return false;
	}
}

/**
 * 
 * @param $ptoid
 * @param $i
 * @return unknown
 */
function verifica_macroetapa( $ptoid , $contador )
{
	global $db;
	
	// esta funcao tem por objetivo numerar os filhos da macro-etapa //
	$sql = "select p.ptoid as ptoidfilho,ptotipo, p.ptoordem,p.ptoordem2
		from monitora.planotrabalho p where p.ptostatus='A' and p.ptoid_pai=$ptoid and pjeid=".$_SESSION['pjeid']."
		order by p.ptodata_ini,p.ptotipo,p.ptodata_fim,p.ptodsc";
	$rs = $db->carregar( $sql );
	if ($rs!=false) {
		foreach ( $rs as $registro ){
			//dbg($registro);
			extract($registro);
			$sql = "select ptoid as ptoidfilho, ptocod, ptotipo
					from monitora.planotrabalho
					where ptoid=$ptoidfilho and pjeid=".$_SESSION['pjeid'];
			$filho = $db->pegaLinha($sql);
			$contador++;
			//dbg($sql);
			$sql = "update monitora.planotrabalho set ptoordem2 = $contador where ptoid = $ptoidfilho";
			$db->executar($sql);
			if ($filho['ptotipo']=='M' ) {
				$contador=verifica_macroetapa($ptoidfilho,$contador);
			}
		}
	}
	return $contador;
}

/**
 * esta funcao tem por objetivo ajustar a ordem das atividades de modo a fazer com que
 * toda a lista fique coerente com sua forma de apresentacao
 * 
 * @return void
 */
function ajusta_ordem( $ptotipo , $ptoordem , $ptoordem2  )
{
	global $db;
	$sql = "update monitora.planotrabalho set ptoordem2=null where pjeid=".$_SESSION['pjeid'];
	//dbg( $sql );
	$db->executar($sql);
	$sql = "select p.ptoid,ptotipo, p.ptoordem  from monitora.planotrabalho p where p.ptostatus='A' 
       and p.ptoid in (select ptoid from monitora.plantrabpje where pjeid=".$_SESSION['pjeid']." and ptoid_pai is null) 
          order by p.ptodata_ini,p.ptotipo,p.ptodata_fim,p.ptodsc";
	// ordeno pelas datas de inicio e fim e pelo tipo	//
	$rs = @$db->carregar( $sql );
	if (  $rs && count($rs) > 0 )
	{
		$contador = 1;
		$i=0;
		foreach ( $rs as $linha )
		{
			foreach($linha as $k=>$v) ${$k}=$v;
			$sql = "select ptocod, ptotipo from monitora.planotrabalho where ptoid=$ptoid and pjeid=".$_SESSION['pjeid'];
			$registro=$db->pegaLinha($sql);
			extract($registro);
			
			$sql = "update monitora.planotrabalho set ptoordem2 = $contador where ptoid = $ptoid";
			//dbg($sql);
			$db->executar($sql);
			
			if ($ptotipo=='M' ){
				$contador=verifica_macroetapa($ptoid,$contador)+1;
			} else {
				$contador++;
			}
		}
	}
	$sql = "update monitora.planotrabalho set ptoordem=ptoordem2 where pjeid=". $_SESSION['pjeid'];
	$db->executar($sql);
	$db->commit();
}

/**
 * Uma etapa não pode ser predecessora, diretamente ou não, de si mesma.	
 * 
 * @param $ptoid
 * @return void
 */
function elimina_antecessor_recurssivo( $ptoid , $ptoid2=null, $ptoordem=null, $antec=null)
{
	global $db;
	$sql = "select ptoordem,ptoid_antecessor as antec from monitora.planotrabalho where ptoid=$ptoid";
	$res=$db->pegalinha($sql);
	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

	if ($antec)
	{
		$sql = "select ptoid as ptoid2 from monitora.planotrabalho where ptoid_antecessor=$ptoordem and ptoordem=$antec and pjeid=".$_SESSION['pjeid'];
		$ptoid2=$db->pegaum($sql);
		if ($ptoid2)
		{
			// entao aconteceu a recursividade
			$sql = "update monitora.planotrabalho set ptoid_antecessor =null where ptoid=$ptoid2";
			$db->executar($sql);
			$db->commit();

		}
	}
}


/**
 * Verifica se existe necessidade de alteração nas datas de início e/ou fim do Projeto Especial
 * caso positivo, verifica se as datas do projeto não estão congeladas.
 * As datas passadas na chamada da função são as datas de íncio e término da atividade
 * 
 * @param integer pjeid
 * @param string dt_ini
 * @param string dt_fim
 * @return boolean
*/
function verifica_data_projetoespecial( $pjeid, $dt_ini, $dt_fim )
{
	global $db;
	$sql = "select to_char(pjedataini,'dd/mm/yyyy') as pjedataini, to_char(pjedatafim,'dd/mm/yyyy') as pjedatafim, pjesndatafechada from monitora.projetoespecial where pjeid=".$pjeid;
	$resultado = $db->pegaLinha( $sql );
	if( is_array( $resultado ) )
	{
		$mudaInicio = verifica_data_maior( $resultado[ 'pjedataini' ], $dt_ini  ) ? TRUE : FALSE;
		$mudaFim = verifica_data_maior( $dt_fim, $resultado[ 'pjedatafim' ] ) ? TRUE : FALSE;
		//Se as datas do projeto especial estiverem congeladas retorna falso (erro )
		if( $resultado[ 'pjesndatafechada' ] == 't' && ( $mudaInicio || $mudaFim ) )
		{
			return FALSE;
		}
		if( $resultado[ 'pjesndatafechada' ] == 'f' && ( $mudaInicio || $mudaFim ) )
		{
			$dataini = $mudaInicio ? date( 'Y-m-d', gera_timestamp( $dt_ini ) ) : FALSE;
			$datafim = $mudaFim ? date( 'Y-m-d',gera_timestamp( $dt_fim ) ) : FALSE;
			$sql = "update monitora.projetoespecial set ";
			$sql .= $dataini ? "pjedataini = '". $dataini ."'" : '';
			$sql .= $dataini && $datafim ? ', ' : '';
			$sql .= $datafim ? "pjedatafim = '". $datafim ."'" : '';
			$sql .= " where pjeid=".$pjeid;
			return (boolean) $db->executar( $sql );
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

/**
 * Verifica se a data1 é maior que a data2
 * As datas devem estar no formato dd/mm/yyyy
 * 
 * @param string data1
 * @param string data2
 * @return boolean
 */
function verifica_data_maior( $data1, $data2 )
{
	$arrAux = explode( '/', $data1 );
	$diaData1 = $arrAux[ 0 ];
	$mesData1 = $arrAux[ 1 ];
	$anoData1 = $arrAux[ 2 ];
	$arrAux = explode( '/', $data2 );
	$diaData2 = $arrAux[ 0 ];
	$mesData2 = $arrAux[ 1 ];
	$anoData2 = $arrAux[ 2 ];
	$tsData1 = mktime( 0, 0, 0, $mesData1, $diaData1, $anoData1 );
	$tsData2 = mktime( 0, 0, 0, $mesData2, $diaData2, $anoData2 );
	if( $tsData1 > $tsData2 )
	return TRUE;

	return FALSE;
}
/**
 * Retorna um timestamp.
 * Data no formato dd/mm/yyyy
 * 
 * @param string data
 * @return integer
 */
function gera_timestamp( $data )
{
	$arrAux = explode( '/', $data );

	return mktime( 0, 0, 0, $arrAux[ 1 ], $arrAux[ 0 ], $arrAux[ 2 ] );
}

/**
 * Verifica se a atividade possui uma atividade antecessora. 
 * Caso positivo, verifica se as datas da atividade podem ser 
 * alteradas de acordo com a data de término da antecessora.
 * 
 * @param integer ptoid
 * @param string dt_ini
 * @param string dt_fim
 */
function verifica_antecessor( $ptoid, $dt_ini, $dt_fim )
{
	global $db;
	$sql = "select ptoid_antecessor from monitora.planotrabalho where ptoid=".$ptoid;
	$ptoid_antecessor = $db->pegaUm( $sql );

	if( $ptoid_antecessor )
	{
		$sql = "select to_char(ptodata_ini, 'dd/mm/yyyy') as dataini, to_char(ptodata_fim, 'dd/mm/yyyy') as datafim from monitora.planotrabalho where ptostatus='A' and ptoordem=".$ptoid_antecessor." and pjeid=".$_SESSION[ "pjeid" ];
		$resultado = $db->pegaLinha( $sql );
		if( $resultado )
		{
			if( verifica_data_maior( $resultado[ 'datafim' ], $dt_ini  ) )
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;
}

/**
 * Verifica se a atividade possui uma atividade pai.
 * Caso positivo, analisa a necessidade de se alterar as datas da atividade pai.
 * Realiza todo o processo de alteração de data para a atividade pai recursivamente.
 * 
 * @param integer pto_id
 * @param string dt_ini
 * @param string dt_fim
 * @return boolean
 */
function verifica_pai( $ptoid, $dt_ini, $dt_fim )
{
	global $db;
	$sql = "select ptoid_pai from monitora.planotrabalho where ptoid=".$ptoid." and pjeid=".$_SESSION[ 'pjeid' ];
	$ptoid_pai = $db->pegaUm( $sql );
	if( $ptoid_pai )
	{
		$sql = "select to_char(ptodata_ini, 'dd/mm/yyyy') as dataini, to_char(ptodata_fim, 'dd/mm/yyyy') as datafim, ptosndatafechada from monitora.planotrabalho where ptoid=".$ptoid_pai;
		$resultado = $db->pegaLinha( $sql );
		$mudaDataIni = verifica_data_maior( $resultado[ 'dataini' ], $dt_ini ) ? TRUE : FALSE;
		$mudaDataFim = verifica_data_maior( $dt_fim, $resultado[ 'datafim' ] ) ? TRUE : FALSE;
		if( ( $mudaDataFim || $mudaDataIni ) && $resultado[ 'ptosndatafechada' ] == 't' )
		{
			return FALSE;
		}
		if( ( $mudaDataFim || $mudaDataIni ) && $resultado[ 'ptosndatafechada' ] == 'f' )
		{
			$dataIni = $mudaDataIni ? $dt_ini : $resultado[ 'dataini' ];
			$dataFim = $mudaDataFim ? $dt_fim : $resultado[ 'datafim' ];
			if( !verifica_antecessor( $ptoid_pai, $dataIni, $dataFim ) || !verifica_pai( $ptoid_pai, $dataIni, $dataFim ) || !verifica_sucessor( $ptoid_pai, $dataIni, $dataFim ) )
			{
				return FALSE;
			}

			$dataIni = $mudaDataIni ? date( 'Y-m-d', gera_timestamp( $dt_ini ) ) : FALSE;
			$dataFim = $mudaDataFim ? date( 'Y-m-d',gera_timestamp( $dt_fim ) ) : FALSE;
			$sql = "update monitora.planotrabalho set ";
			$sql .= $dataIni ? "ptodata_ini = '". $dataIni ."'" : '';
			$sql .= $dataIni && $dataFim ? ', ' : '';
			$sql .= $dataFim ? "ptodata_fim = '". $dataFim ."'" : '';
			$sql .= " where ptoid=".$ptoid_pai;

			return (boolean) $db->executar( $sql );
		}
	}

	return TRUE;
}

/**
 * Verifica se a atividade possui atividades filhas. 
 * Caso positivo, analisa a necessidade de se alterar as datas de cada atividade filha.
 * Realiza todo o processo de alteração de data para as atividades filhas recursivamente.
 * 
 * @param integer pto_id
 * @param string dt_ini
 * @param string dt_fim
 * @return boolean 
 */
function verifica_filhos( $ptoid, $dt_ini, $dt_fim )
{
	global $db;
	$sql = "select ptoid, to_char(ptodata_ini, 'dd/mm/yyyy') as dataini, to_char(ptodata_fim, 'dd/mm/yyyy') as datafim, ptosndatafechada from monitora.planotrabalho where ptoid_pai=".$ptoid." and pjeid=".$_SESSION[ 'pjeid' ];
	$resultado = $db->carregar( $sql );
	if( $resultado )
	{
		foreach( $resultado as $linha )
		{
			$mudaDataIni = verifica_data_maior( $dt_ini, $linha[ 'dataini' ] ) ? TRUE : FALSE;
			$mudaDataFim = verifica_data_maior( $linha[ 'datafim' ], $dt_fim ) ? TRUE : FALSE;

			if( ( $mudaDataIni || $mudaDataFim ) && $linha[ 'ptosndatafechada' ] == 't' )
			{
				return FALSE;
			}
			if( ( $mudaDataIni || $mudaDataFim ) && $linha[ 'ptosndatafechada' ] == 'f' )
			{
				$dataIni = $mudaDataIni ? $dt_ini : date( $linha[ 'dataini' ] );
				$dataFim = $mudaDataFim ? $dt_fim : $linha[ 'datafim' ];
				if( !verifica_antecessor( $linha[ 'ptoid' ], $dataIni, $dataFim ) || !verifica_filhos( $linha[ 'ptoid' ], $dataIni, $dataFim ) || !verifica_sucessor( $linha[ 'ptoid' ], $dataIni, $dataFim )  )
				{
					return FALSE;
				}
				$dataIni = $mudaDataIni ? date( 'Y-m-d', gera_timestamp( $dt_ini ) ) : FALSE;
				$dataFim = $mudaDataFim ? date( 'Y-m-d',gera_timestamp( $dt_fim ) ) : FALSE;
				$sql = "update monitora.planotrabalho set ";
				$sql .= $dataIni ? "ptodata_ini = '". $dataIni ."'" : '';
				$sql .= $dataIni && $dataFim ? ', ' : '';
				$sql .= $dataFim ? "ptodata_fim = '". $dataFim ."'" : '';
				$sql .= " where ptoid=".$linha[ 'ptoid' ];

				if( !$db->executar( $sql ) )
				{
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}

/**
 * Verifica se a atividade possui atividades sucessoras. 
 * Caso positivo, analisa a necessidade de se alterar as datas da atividade sucessora.
 * Realiza todo o processo de alteração de data para cada atividade sucessora.
 * A duração da atividade sucessora é mantida, ou seja, caso a data de início seja prorrogada, 
 * a de término também o será.
 * 
 * @param integer pto_id
 * @param string dt_ini
 * @param string dt_fim
 * @return boolean
 */
function verifica_sucessor( $ptoid, $dt_ini, $dt_fim )
{
	global $db;
	$sql = "select ptoordem from monitora.planotrabalho where ptoid=".$ptoid;
	$ptoordem = $db->pegaUm( $sql );

	$sql = "select ptoid, to_char(ptodata_ini, 'dd/mm/yyyy') as dataini, to_char(ptodata_fim, 'dd/mm/yyyy') as datafim, ptosndatafechada from monitora.planotrabalho where ptoid_antecessor=".$ptoordem." and pjeid=".$_SESSION[ 'pjeid' ];
	$resultado = $db->carregar( $sql );
	if( $resultado )
	{
		foreach( $resultado as $linha )
		{
			$mudaDataIni = verifica_data_maior( $dt_fim, $linha[ 'dataini' ] ) ? TRUE : FALSE;
			if( $mudaDataIni && $linha[ 'ptosndatafechada' ] == 't' )
			{
				return FALSE;
			}
			if( $mudaDataIni && $linha[ 'ptosndatafechada' ] == 'f' )
			{
				$tsDataIni = gera_timestamp( $linha[ 'dataini' ] );
				$tsDataFim = gera_timestamp( $linha[ 'datafim' ] );
				$intervalo = $tsDataFim - $tsDataIni;//duracao da atividade

				$dataIni = date( "d/m/Y", gera_timestamp( $dt_fim ) );
				$dataFim = date( "d/m/Y", gera_timestamp( $dt_fim ) + $intervalo );
				if( !verifica_pai( $linha[ 'ptoid' ], $dataIni, $dataFim ) || !verifica_filhos( $linha[ 'ptoid' ], $dataIni, $dataFim ) || !verifica_sucessor( $linha[ 'ptoid' ], $dataIni, $dataFim ) )
				{
					return FALSE;
				}
				$dataIni = date( "Y-m-d", gera_timestamp( $dt_fim ) );
				$dataFim = date( "Y-m-d", gera_timestamp( $dt_fim ) + $intervalo );
				$sql = "update monitora.planotrabalho set ptodata_ini='".$dataIni."', ptodata_fim='".$dataFim."' where ptoid=".$linha[ 'ptoid' ];
				if( !$db->executar( $sql ) )
				{
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}

/**
 * 
 * @param $ptoid
 * @param $dt_ini
 * @param $dt_fim
 * @return boolean
 */
function altera_data_atividade( $ptoid, $dt_ini, $dt_fim )
{
	global $db;

	$sql = "select ptosndatafechada from monitora.planotrabalho where ptoid=".$ptoid;
	$dtFechada = $db->pegaUm( $sql );
	if( $dtFechada == 'f' )
	{
		$dataIni = date( "Y-m-d", gera_timestamp( $dt_ini ) );
		$dataFim = date( "Y-m-d", gera_timestamp( $dt_fim ) );
		$sql = "update monitora.planotrabalho set ptodata_ini='".$dataIni."', ptodata_fim='".$dataFim."' where ptoid=".$ptoid;

		return (boolean) $db->executar( $sql );
	}

	return FALSE;
}

/**
 * 
 * @param $array
 * @param $valor
 * @return boolean
 */
function existe_no_array( $array, $valor )
{
	for( $i = 0 ; $i < count( $array ) ; $i++ )
	{
		if( $valor == $array[ $i ] )
		return TRUE;
	}

	return FALSE;
}

/**
 * Faz as verificações necessárias para alteração de data da atividade.
 * A verificação retornará erro tão logo ele seja detectado e o processo de verificação será interrompido.
 * Ordem das verificações
 * 1 - Projeto Especial
 * 2 - Atividade antecessora
 * 3 - Atividade pai (Macro-Etapa agregadora)
 * 4 - Atividades filhas (Macro-Etapas/Etapas agregadas)
 * 5 - Atividades sucessoras
 * 
 * Esta função retornará um inteiro, a saber:
 * 0 - Sem erros
 * 1 - Erro no processo de modificação das datas do Projeto Especial.
 * 2 - Erro na verificação da atividade antecessora. As datas da atividade antecessora não podem ser alteradas.
 * 3 - Erro na alteração das datas da Macro-Etapa agregadora. 
 * Pode ser qualquer uma atividade pertencente à árvore de atividades em questão, 
 * desde que esteja em níveis superiores.
 * 4 - Erro na alteração das datas de uma das Macro-Etapas/Etapas agregadas. 
 * Pode ser qualquer uma atividade pertencente à árvore de atividades em questão, 
 * desde que esteja em níveis inferiores.
 * 5 - Erro na alteração das datas de uma das atividades sucessoras. 
 * Ao se alterar a data de início da atividade sucessora, 
 * a duração da atividade será mantida, ou seja, o sistema alterará a data de término da atividade.
 * 
 * @param integer ptoid
 * @param string dt_ini
 * @param string dt_fim
 * @return integer
 */
function verifica_datas_atividade( $ptoid, $dt_ini, $dt_fim )
{
	if( !verifica_data_projetoespecial( $_SESSION[ 'pjeid' ], $dt_ini, $dt_fim ) )
	{
		return 1;
	}
	if( !verifica_antecessor( $ptoid, $dt_ini, $dt_fim ) )
	{
		return 2;
	}
	if( ( !verifica_pai( $ptoid, $dt_ini, $dt_fim ) ) )
	{
		return 3;
	}
	if( !verifica_filhos( $ptoid, $dt_ini, $dt_fim ) )
	{
		return 4;
	}
	if( !verifica_sucessor( $ptoid, $dt_ini, $dt_fim ) )
	{
		return 5;
	}
	return 0;
}


/**
 * Segue o mesmo padrão para criação do combo_popup. 
 * Porém, esta função também dá a opção de inserir novos valores manualmente.
 * Por enquanto o suporte a inserção de novos valores está atrelado apenas a dados de desembolso (valor e data).
 * Os valores aqui criados apenas serão inseridos no banco após o usuário submeter o formulário principal.
 * 
 * @param string nome
 * @param string titulo
 * @param string tamanho_janela
 * @param string habilitado
 */
function combo_desembolso( $nome, $titulo, $tamanho_janela = '400x400', $habilitado = 'S' )
{
	// , $limite = 10 , $maximo_itens = 10
	global ${$nome};
	global $limite;
	global $maximo_itens;
	// prepara sessao
	$dados_sessao = array(
	'titulo' => $titulo,
	'limite' => $limite
	);
	$_SESSION['indice_sessao_combo_desembolso'][$nome] = $dados_sessao;
	// monta html para formulario
	$tamanho = explode( 'x', $tamanho_janela );
	$onclick = ' onclick="javascript:combo_desembolso_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ' );" ';
	$habilitado = $habilitado == 'S' ? '' : ' disabled="disabled" ' ;		
	print '<select tipo="combo_desembolso" multiple="multiple" size="5" name="' . $nome . '[]" id="' . $nome . '" ' . $onclick . ' class="CampoEstilo" style="width:400px;" ' . $habilitado . '>';
	
	if ( ${$nome} && count( ${$nome} ) > 0 )
	{
		$itens_criados = 0;
		foreach ( ${$nome} as $item )
		{
			print '<option value="' . $item["data"] . ' - ' . $item[ "valor" ] . '">' . $item["data"] . ' - R$ ' . $item[ "valor" ] . '</option>';
			$itens_criados++;
			if ( $maximo_itens != 0 && $itens_criados >= $maximo_itens )
			{
				break;
			}
		}
	}
	else
	{
		print '<option value="">Clique Aqui para Selecionar</option>';
	}		
	print '</select>';
}

/**
 * 
 * @param $ptoid
 * @param $dados
 * @return void
 */	
function inserirDesembolso( $ptoid, $dados )
{
	global $db;
	$sql = "delete from monitora.desembolso_projeto where ptoid=".$ptoid;
	
	$db->executar( $sql );
	foreach( (array) $dados as $i => $dado )
	{
		if( $dado != '' )
		{
			$arrAux = explode( ' - ', $dado );
			$tsData = gera_timestamp( $arrAux[ 0 ] );
			$dt = date( 'Y-m-d', $tsData );
			$valor = $arrAux[ 1 ];
			$sql = " insert into monitora.desembolso_projeto(ptoid, dpedata, dpevalor)" .
				   " values( " . $ptoid . ", '" . $dt . "', '" . $valor . "' )";
			$db->executar( $sql );
		}
	}
}

/**
 * 
 * @param $ptoid
 * @return number
 */
function pega_saldo_total( $ptoid )
{
	global $db;
	$sql = "select sum( dpevalor ) as total from monitora.desembolso_projeto where ptoid=".$ptoid;
	$total = $db->pegaUm( $sql );
	
	$totalFilhos = pega_saldo_filhos( $ptoid );		
	return $total - $totalFilhos;
}

/**
 * 
 * @param $id
 * @return number
 */
function pega_saldo_filhos( $id )
{
	global $db;
	global $ptoid;
	$sql = "select ptoid from monitora.planotrabalho where ptoid_pai=".$id;
	$rsFilhos = $db->carregar( $sql );
	$totalFilhos = 0;
	if( $rsFilhos )
	{			
		foreach( $rsFilhos as $linha )
		{
			if( $ptoid != $linha[ 'ptoid' ] )
			{
				$sql = "select sum( dpevalor ) as total from monitora.desembolso_projeto where ptoid=".$linha[ 'ptoid' ];
				$total = $db->pegaUm( $sql );
				$totalFilhos += $total;					
			}
		}
	}
	
	return $totalFilhos;
}

function inserirPlanoTrabalho( $ptotipo , $ptoordem , $ptoordem2 , $ungabrev )
{
	global $db;
	
	$_REQUEST['ptoordem']=0;
	$pjeid=$_SESSION['pjeid'];
	
	// valores default //
	
	if (! $_REQUEST['ptosnsoma'] )				$_REQUEST['ptosnsoma']				= 'f';
	if (! $_REQUEST['ptosnaprovado'] )			$_REQUEST['ptosnaprovado']			= 'f';
	if (! $_REQUEST['ptodescricao'] )			$_REQUEST['ptodescricao']			= null;
	if (! $_REQUEST['ptofinalidade'] )			$_REQUEST['ptofinalidade']			= null;
	if (! $_REQUEST['ptodetalhamento'] )		$_REQUEST['ptodetalhamento']		= null;
	if (! $_REQUEST['ptoid_pai'] )				$_REQUEST['ptoid_pai']				= 'null';
	if (! $_REQUEST['ptoanofim'] )				$_REQUEST['ptoanofim']				= $_SESSION['exercicio'];
	if (! $_REQUEST['ptoavisoantecedencia'] )	$_REQUEST['ptoavisoantecedencia']	= 7;
	if (! $_REQUEST['usucpf'] )					$_REQUEST['usucpf']					= null;
	if (! $_REQUEST['ptoid_antecessor'] )	$_REQUEST['ptoid_antecessor']	= 'null';
	if (! $_REQUEST['ptosndatafechada'] )		$_REQUEST['ptosndatafechada']		= 'f';
	if (! $_REQUEST['ptosntemdono'] )			$_REQUEST['ptosntemdono']			= 'f';

	// usuario do plano de trabalho //	
	if ( $_REQUEST['usucpf'] <> null)
	{
		$strSqlUsuario = "'" . $_REQUEST['usucpf'] . "',";
	}
	else
	{
		$strSqlUsuario = "null" . ",";
	}
	
	// gerando a query //
	
	$sql =	' INSERT INTO ' . 'monitoria.planotrabalho' .
				' ( ' .
					' ptostatus , '.
					' ptodescricao , ' .
					' ptofinalidade , ' .
					' ptodetalhamento , ' .
					' ptotipo , ' .
					' ptodsc , ' .
					' ptocod , ' . 
					' ptoprevistoexercicio , ' . 
					' unmcod , ' . 
					' ptosnsoma , ' . 
					' ptosnaprovado , ' . 
					' ptodata_ini , ' . 
					' ptodata_fim , ' . 
					' ptoordem , ' . 
					' ptoid_pai , ' . 
					' ptoanofim , ' . 
					' ptoavisoantecedencia , ' . 
					' pjeid , ' . 
					' usucpf , ' . 
					' ptoid_antecessor , ' . 
					' ptosndatafechada , ' . 
					' ptosntemdono  ' . 
				' ) ' .
			' VALUES ' .
				'( ' .
					"'A'" . "," .
					"'" . str_to_upper( $_REQUEST[ 'ptodescricao' ] ) . "'," .
					"'" . str_to_upper( $_REQUEST[ 'ptofinalidade' ] ) . "'," .
					"'" . str_to_upper($_REQUEST['ptodetalhamento'])."',".
					"'" . str_to_upper($_REQUEST['ptotipo'])."',".
					"'" . str_to_upper($_REQUEST['ptodsc'])."',".
					"'" . str_to_upper($_REQUEST['ptocod'])."',".
					"'" . $_REQUEST[ 'ptoprevistoexercicio' ] . "',".
					"'" . $_REQUEST[ 'unmcod' ] . "',".
					"'" . $_REQUEST[ 'ptosnsoma' ] . "',".
					"'" . $_REQUEST[ 'ptosnaprovado' ] ."',".
					"'" . $_REQUEST[ 'ptodata_ini' ] ."',".
					"'" . $_REQUEST[ 'ptodata_fim' ] ."'," . 
					"0" . "," .
					$_REQUEST[ 'ptoid_pai' ] ."," .
					"'" . $_REQUEST[ 'ptoanofim' ] ."',".
					$_REQUEST[ 'ptoavisoantecedencia'] .",".
					$_SESSION[ 'pjeid'] . "," .
					$strSqlUsuario .
					$_REQUEST['ptoid_antecessor'] . "," . 
					"'" . $_REQUEST['ptosndatafechada'] . "'" . "," . 
					"'" . $_REQUEST['ptosntemdono'] . "'" . 
				' ) ' .
			'';

	// executando a query criada //
	
	$saida = $db->executar($sql);
	
/*
	// if desnecessario sendo que a query é executada de qualquer modo. //
	// mantido apenas enquanto se faz a checagem de erros				//
	if( $saida )
	{
		$sql = "select ptoid from monitora.planotrabalho where oid=".pg_last_oid( $saida );
		$ultimoPtoid = $db->pegaUm( $sql );
	}
*/
	// pega o ultimo inserido //
	
	$sql =	' SELECT ' . 
				'ptoid' . 
			' FROM ' . 
				'monitora.planotrabalho' . 
			' WHERE '.
				'oid' . ' = ' . pg_last_oid($saida) .
			'';
		
	$ptoid = $db->pegaUm($sql);
	$ultimoPtoid = $ptoid;

	$sql =	'INSERT INTO ' .
				'monitora.plantrabpje ' .
				' ( ' . 
					'ptoid' . 
					',' . 
					'pjeid' .
				' ) ' .
			' VALUES '.
				' ( ' .
					$ptoid . ',' . 
					$_SESSION[ 'pjeid' ] . 
				' ) ' .
			'';
			
	$saida = $db->executar($sql);

	switch( verifica_datas_atividade( $ultimoPtoid, $_REQUEST[ 'ptodata_ini' ], $_REQUEST[ 'ptodata_fim' ] ) )
	{
		case 0:
			if ($_REQUEST['usucpf'] && $_REQUEST['usucpf'] <>null )
			{
				// inclui o usuário na tabela de usuário responsabilidade
	
				$sql =	' INSERT INTO ' .
							'monitora.usuarioresponsabilidade' .
							' ( ' .
								'pjeid' . 
								',' . 
								'usucpf' . 
								',' . 
								'pflcod' . 
								',' . 
								'prsano' .
							' ) ' .
						' VALUES' .
							' ( ' .
								$pjeid . ', ' . 
								'\'' . $_REQUEST['usucpf']. '\'' . ',' .
								51 . ',' . // @TODO descobrir que numero magico eh esse
								'\'' . $_SESSION['exercicio']. '\'' . 
							' ) ' .
							
				$db->executar($sql);
				// verifica se o usuario possui o perfil 51 na tabela perfilusuario
				
				$sql =	' SELECT '.
							'usucpf' . 
						' FROM ' .
							'seguranca.perfilusuario' . 
						' WHERE ' .
							'pflcod' . ' = '  . 51 . // @TODO descobrir que numero magico eh esse
						' AND ' .
							'usucpf' . '=' . '\'' . $_REQUEST['usucpf'] . '\'' .
						'';
						
				if (! $db->pegaUm($sql))
				{
					$sql .= ' INSERT INTO '.
								'seguranca.perfilusuario' .
								' ( ' .
									'pflcod' . 
									',' . 
									'usucpf' .
								' ) ' .
							'VALUES' .
								' ( ' .
									51 . ',' . 
									'\'' . $_REQUEST[ 'usucpf' ] . '\'' . 
								' ) ' .
							'';
					$db->executar($sql);
				}

				 atualiza_ator($pjeid,$_REQUEST['usucpf'],'A','D');  
				 
				// envia email para o responsável avisando o ocorrido
	            if ($_REQUEST['usucpf']<>null)
	            {
				if (str_to_upper($_REQUEST['ptotipo'])=='M') $tipo='Macro-etapa' ; else $tipo='Etapa';
				$cod=str_to_upper($_REQUEST['ptocod']);
				$desc=str_to_upper($_REQUEST['ptodsc']);

				$sql =	' SELECT ' .
							'ug.ungabrev ' . 
							',' . 
							' p.pjecod ' . 
							',' . 
							' p.pjedsc ' .
						' FROM ' .
						 	'  monitora.projetoespecial p' .
						' INNER JOIN ' .
						 	' unidadegestora ug'.
						' ON ' .
							' ug.ungcod' . ' = ' . 'p.ungcod' . 
						' WHERE ' . 
							' p.pjeid' . ' = ' . $_SESSION['pjeid'] .
						'';
				
				$res=$db->pegalinha($sql);
				if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
				
				// envia email
				$assunto = 'Inclusão como responsável em Etapa ou Macro Etapa em Projeto Especial';
				$sexo = 'Prezado Sr.  ';
				
				$sql =	' SELECT ' .
							'ususexo' . 
							',' . 
							'usunome' . 
							',' . 
							'usuemail' . 
						' FROM ' . 
							'seguranca.usuario' . 
						' WHERE ' . 
							'usucpf' . ' = ' . '\'' . $_REQUEST['usucpf'] . '\'';
							
				$res=$db->pegalinha($sql);
				if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
				if ($ususexo == 'F') $sexo = 'Prezada Sra. ';
	
				$mensagem = $sexo. str_to_upper($usunome).chr(13).
				"Reportamos que seu nome foi associado, no SIMEC, como responsável de uma $tipo, identificada como $cod - $desc do Projeto Especial ".$ungabrev.$pjecod.' - '.$pjedsc;
				email($usunome, $usuemail, $assunto, $mensagem);
			}
			}
			elimina_antecessor_recurssivo($ultimoPtoid);
			ajusta_ordem( $ptotipo , $ptoordem , $ptoordem2 );
			inserirDesembolso( $ultimoPtoid, $_REQUEST[ 'ptovlrprevisto' ] );
			$db->commit();
				$db->sucesso($modulo);
			//header( "Location: ?modulo=principal/projespec/plantrabpje&acao=I&act2=alterar&ptoid=".$ultimoPtoid);
			//exit();
			break;
		case 1:
			$strErro = "Erro ao tentar alterar as datas do projeto especial.";
			break;
		case 2:
			$strErro = "As datas da atividade antecessora não podem ser modificadas.";
			break;
		case 3:
			$strErro = "Erro ao tentar mudar as datas da Macro-Etapa agregadora.";
			break;
		case 4:
			$strErro = "Erro ao tentar mudar as datas das Macro-Etapas/Etapas agregadas.";
			break;
		case 5:
			$strErro = "Erro ao tentar mudar as datas das Macro-Etapas/Etapas sucessoras.";
			break;
	}
}

function alterarPlanoTrabalho( $ptotipo , $ptoordem , $ptoordem2 , $ungabrev )
{
	global $db;
	// fazer alteração da atividade.

	$pjeid = $_SESSION['pjeid'];
	$ptoid = $_REQUEST['ptoid'];
	
	if (! $_REQUEST['ptosnsoma'] ) 				$_REQUEST['ptosnsoma']			=	'f';
	if (! $_REQUEST['ptosnaprovado'] ) 			$_REQUEST['ptosnaprovado']		=	'f';
	if (! $_REQUEST['ptodescricao'] )			$_REQUEST['ptodescricao']		=	null;
	if (! $_REQUEST['ptofinalidade'] )			$_REQUEST['ptofinalidade']		=	null;
	if (! $_REQUEST['ptodetalhamento'] )		$_REQUEST['ptodetalhamento']	=	null;
	if (! $_REQUEST['ptoid_pai'] )				$_REQUEST['ptoid_pai']			=	'null';
	if (! $_REQUEST['ptoanofim'] ) 				$_REQUEST['ptoanofim']			=	$_SESSION['exercicio'];
	if (! $_REQUEST['ptoavisoantecedencia'] )	$_REQUEST['ptoavisoantecedencia'] = 7;
	if (! $_REQUEST['usucpf'] )					$_REQUEST['usucpf']				= null;
	if (! $_REQUEST['ptoid_antecessor'] )	$_REQUEST['ptoid_antecessor']='null';
	if (! $_REQUEST['ptosndatafechada'] )		$_REQUEST['ptosndatafechada']	='f';
	if (! $_REQUEST['ptosntemdono'] )			$_REQUEST['ptosntemdono']		='f';

	if (! $_REQUEST['ptoordem'] or $_REQUEST['ptoordem'] == 0)
	{
		$sql =	''.
				' SELECT ' . 
					'ptoordem' . 
				' FROM ' .
					'monitora.planotrabalho' .
				' WHERE ' .
					'ptoid' . ' = ' . $ptoid .
				'';
		$_REQUEST['ptoordem'] = $db->pegaUm($sql);
	}
	
	$sql = ''.
		' SELECT ' . 
			'usucpf' . 
		' FROM ' . 
			'monitora.planotrabalho' . 
		' WHERE ' .
			'ptoid' . ' = ' . $_REQUEST['ptoid'];
			
	$cpforiginal=$db->pegaum($sql);
	
	if ( $_REQUEST[ 'usucpf' ]  != null)
	{
		$usuCpfValue = $_REQUEST['usucpf'];
	}
	else
	{
		$usuCpfValue = 'null';
	}

	$sql =	' UPDATE ' .
				'monitora.planotrabalho' .
			' SET ' .
				'ptocod' . ' = ' . '\'' . $_REQUEST['ptocod']. '\'' . 
				',' . 
				'ptodetalhamento' . ' = ' . '\'' . $_REQUEST['ptodetalhamento']. '\'' . 
				',' . 
				'ptofinalidade' . ' = ' . '\'' .$_REQUEST['ptofinalidade'] . '\'' . 
				',' . 
				'ptodescricao' . ' = ' . '\'' . $_REQUEST['ptodescricao'] . '\'' . 
				',' . 
				'ptodsc' . ' = ' . '\'' . str_to_upper( $_REQUEST['ptodsc'] ) . '\'' . 
				',' . 
				'ptoprevistoexercicio' . ' = ' . '\'' . $_REQUEST['ptoprevistoexercicio'] . '\'' . 
				',' . 
				'unmcod' . ' = ' . '\'' . $_REQUEST['unmcod'] . '\'' . 
				',' . 
				'ptosnsoma' . ' = ' . '\'' .$_REQUEST['ptosnsoma'] . '\'' . 
				',' . 
				'ptodata_ini' . ' = ' . '\'' .$_REQUEST['ptodata_ini'] . '\'' . 
				',' . 
				'ptodata_fim' . ' = ' . '\'' .$_REQUEST['ptodata_fim'] . '\'' . 
				',' . 
				'ptoid_pai' . ' = ' . $_REQUEST['ptoid_pai'] . 
				',' .
				'ptoanofim' . ' = ' . '\'' . $_REQUEST['ptoanofim'] . '\'' . 
				',' . 
				'ptoavisoantecedencia' . ' = ' . '\'' . $_REQUEST['ptoavisoantecedencia'] . '\'' . 
				',' . 
				'ptoordem' . ' = ' . ( $_REQUEST['ptoordem'] ? $_REQUEST['ptoordem'] : 'NULL' ). 
				',' . 
				'usucpf' . ' = ' . $usuCpfValue . 
				',' .
				'ptoid_antecessor' . ' = ' . $_REQUEST[ 'ptoid_antecessor' ] . 
				',' . 
				'ptosndatafechada' . '=' . '\'' .$_REQUEST['ptosndatafechada']. '\'' . 
				',' .
				'ptosntemdono' . ' = ' . '\'' .$_REQUEST['ptosntemdono'] . '\'' .  
			' WHERE ' .
				'ptoid' . '=' . $_REQUEST['ptoid'] .
			'';

	$saida = $db->executar($sql);
	
	switch( verifica_datas_atividade( $ptoid, $_REQUEST[ 'ptodata_ini' ], $_REQUEST[ 'ptodata_fim' ] ) )
	{
		case 0:
			
		// envia email para o responsável avisando do ocorrido se e somente se for diferente do original.	//
		// Neste caso envia email para os dois.																//
		if ($cpforiginal <> $_REQUEST['usucpf'])
		{
			
			if ($_REQUEST['usucpf'] and $_REQUEST['usucpf'] <> null) {
				// então mudou de responsável

				// inclui o usuário na tabela de usuário responsabilidade
				// apaga registros antigos caso haja
				
				$sql =	' DELETE FROM ' .
							'monitora.usuarioresponsabilidade' . 
						' WHERE ' . 
							'pflcod' . ' = '. 51 . // @TODO descobrir que numero magico eh esse
						' AND ' . 
							'usucpf' . ' = ' . '\'' . $_REQUEST['usucpf'] . '\'' . 
						' AND '. 
							'pjeid' . ' = ' . $_SESSION['pjeid'] .
						'';
				$db->executar($sql);
				
				//inclui um novo
				
				$sql =	' INSERT INTO ' .
							'monitora.usuarioresponsabilidade' . 
							' ( ' . 
								'pjeid' . 
								',' . 
								'usucpf' . 
								',' . 
								'pflcod' . 
								',' . 
								'prsano' . 
							' ) ' . 
						' VALUES ' . 
							' ( ' . 
								$pjeid . 
								',' . 
								'\'' . $_REQUEST['usucpf'] . '\'' . 
								',' . 
								51 .  // @TODO descobrir que numero magico eh esse
								',' . 
								'\'' . $_SESSION['exercicio']. '\'' . 
							' ) ' .
						'';
							

				$db->executar($sql);
				

				// verifica se o usuario possui o perfil 51 na tabela perfilusuario
				$sql =	' SELECT ' .
							'usucpf' .
						' FROM ' .
							'seguranca.perfilusuario' .
						' WHERE ' .
							'pflcod' . ' = '. 51 .  // @TODO descobrir que numero magico eh esse
						' AND ' .
							'usucpf' . '=' . '\'' . $_REQUEST['usucpf']. '\'' .
						'';

				if (! $db->pegaUm($sql))
				{
					
					// insere
					
					$sql =	' INSERT INTO ' . 
								'seguranca.perfilusuario' .
								' ( ' .
									'pflcod' . 
									',' . 
									'usucpf' . 
								' ) ' . 
							'VALUES' . 
								' ( ' . 
									51 .  // @TODO descobrir que numero magico eh esse
									',' . 
									'\'' . $_REQUEST['usucpf']. '\'' . 
								' ) ' .
							'';
					$db->executar($sql);
				}
				
				
				atualiza_ator($pjeid,$_REQUEST['usucpf'],'A','D');
				//$db->commit();

				// agora envia email para o novo responsável
				if (str_to_upper($_REQUEST['ptotipo'])=='M') $tipo='Macro-etapa' ; else $tipo='Etapa';
				$cod=str_to_upper($_REQUEST['ptocod']);
				$desc=str_to_upper($_REQUEST['ptodsc']);
				
				$sql = ' SELECT ' . 
							'ug.ungabrev' . 
							',' . 
							'p.pjecod' . 
							',' . 
							'p.pjedsc' . 
						' FROM ' . 
							'monitora.projetoespecial p' .
						' INNER JOIN ' . 
							'unidadegestora ug' . 
						' ON ' . 
							'ug.ungcod' . ' = ' . 'p.ungcod' . 
						' WHERE ' . 
							'p.pjeid' . ' = ' . $_SESSION['pjeid'] .
						'';

				$res=$db->pegalinha($sql);
				
				if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
				// envia email
				$assunto = 'Inclusão como responsável em Etapa ou Macro Etapa em Projeto Especial';
				$sexo = 'Prezado Sr.  ';
				
				$sql = ' SELECT ' .
							'ususexo' . 
							',' . 
							'usunome' . 
							',' . 
							'usuemail' . 
						' FROM ' .
							'seguranca.usuario' . 
						' WHERE ' . 
							'usucpf' . ' = ' . '\'' . $_REQUEST['usucpf'] . '\'' .
						'';
				
				$res=$db->pegalinha($sql);
				if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
				if ($ususexo == 'F') $sexo = 'Prezada Sra. ';
				$mensagem = $sexo. str_to_upper($usunome).chr(13)."Reportamos que seu nome foi associado, no SIMEC, como responsável de uma $tipo, identificada como $cod - $desc do Projeto Especial ".$ungabrev.$pjecod.' - '.$pjedsc;
				email($usunome, $usuemail, $assunto, $mensagem);
			}
			
			if ($cpforiginal)
			{
				// agora envia email para o que deixou de ser responsavel
				$assunto = 'Exclusão como responsável em Etapa ou Macro Etapa em Projeto Especial';
				$sexo = 'Prezado Sr.  ';
				
				$sql = ' SELECT ' .
							'ususexo' . 
							',' . 
							'usunome' . 
							',' . 
							'usuemail' . 
						' FROM ' .
							'seguranca.usuario' . 
						' WHERE ' . 
							'usucpf' . ' = ' . '\'' . $cpforiginal . '\'' .
						'';
							
				$res=$db->pegalinha($sql);
				if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
				if ($ususexo == 'F') $sexo = 'Prezada Sra. ';
				$mensagem = $sexo. str_to_upper($usunome).chr(13)."Reportamos que seu nome foi retirado, no SIMEC, como responsável de uma $tipo, identificada como $cod - $desc do Projeto Especial ".$ungabrev.$pjecod.' - '.$pjedsc;
				email($usunome, $usuemail, $assunto, $mensagem);
			}
		}
		
		elimina_antecessor_recurssivo($ptoid);
		ajusta_ordem( $ptotipo , $ptoordem , $ptoordem2 );
		inserirDesembolso( $ptoid, $_REQUEST[ 'ptovlrprevisto' ] );
		$db->commit();
		$db->sucesso($modulo);
		break;
		case 1:
		$strErro = "Erro ao tentar alterar as datas do projeto especial.";
		break;
		case 2:
		$strErro = "As datas da atividade antecessora não podem ser modificadas.";
		break;
		case 3:
		$strErro = "Erro ao tentar mudar as datas da Macro-Etapa agregadora.";
		break;
		case 4:
		$strErro = "Erro ao tentar mudar as datas das Macro-Etapas/Etapas agregadas.";
		break;
		case 5:
		$strErro = "Erro ao tentar mudar as datas das Macro-Etapas/Etapas sucessoras.";
		break;

	}
}

function aprovarAtividade( $modulo , $ptotipo, $ptoordem , $ptoordem2  )
{
	global $db;
	// aprovar a atividade.
	$sql =  ' UPDATE ' . 
					'monitora.planotrabalho' . 
			' SET ' . 
				'ptosnaprovado' . ' = ' . '\'' . 't' . '\'' . 
			' WHERE ' . 
				'ptoid' . ' = ' . $_REQUEST['ptoid'];
				
	$saida = $db->executar($sql);
	$db->commit();
	ajusta_ordem( $ptotipo , $ptoordem , $ptoordem2 );
	$db->sucesso( $modulo );
}

function aprovarLotedeAtividades( $modulo , $pjeid )
{
	global $db;
	// aprovar a atividade em lote

	$sql =  ' UPDATE ' . 
					'monitora.planotrabalho' . 
			' SET ' . 
				'ptosnaprovado' . ' = ' . '\'' . 'f' . '\'' . 
			' WHERE ' . 
				'pjeid' . ' = ' . $pjeid;
	
	$saida = $db->executar($sql);
	
	$aprovpto = $_POST['aprovpto'];
    $nlinhas = count($aprovpto)-1;
    for ($j=0; $j<=$nlinhas;$j++)
    {    	
  	   atualiza_aprov($_POST['aprovpto'][$j]);
    }
	$db->commit();

	$db->sucesso($modulo);
}

function retornarAtividadeParaEdicao( $modulo , $ptotipo , $ptoordem , $ptoordem2  )
{
	global $db;
	// retornar a atividade para a edição.
	
	$sql =  ' UPDATE ' . 
					'monitora.planotrabalho' . 
			' SET ' . 
				'ptosnaprovado' . ' = ' . '\'' . 'f' . '\'' . 
			' WHERE ' . 
				'ptoid' . ' = ' . $_REQUEST['ptoid'];
	
	$saida = $db->executar($sql);
	$db->commit();
	ajusta_ordem( $ptotipo , $ptoordem , $ptoordem2 );
	$db->sucesso($modulo);
}


/**
 * Atualiza as datas das atividades, retorna ao encontrar o primeiro erro
 * @return void
 */
function atualizarDatasdasAtividades( $ptotipo , $ptoordem , $ptoordem2 )
{
	global $db;
	$arrCodigos = explode( ',', $_REQUEST[ "arrCod" ] );
	for( $i = 0 ; $i < count( $arrCodigos ) ; $i++ )
	{
		$dt_ini = $_REQUEST[ 'dt_ini'.$arrCodigos[ $i ] ];
		$dt_fim = $_REQUEST[ 'dt_fim'.$arrCodigos[ $i ] ];
		$ultimoId = $arrCodigos[ $i ];
		if( !verifica_data_projetoespecial( $_SESSION[ 'pjeid' ], $dt_ini, $dt_fim ) )
		{
			$erroData = array(
			'ptoid' => $arrCodigos[ $i ],
			'mensagem' => "Erro ao tentar mudar a data do projeto especial de acordo com a data da atividade.\\n Verifique se as datas do projeto especial não estão congeladas."
			);
			break;
		}
		if( !verifica_antecessor( $arrCodigos[ $i ], $dt_ini, $dt_fim ) )
		{
			$erroData = array(
			'ptoid' => $arrCodigos[ $i ],
			'mensagem' => "Erro ao tentar mudar a data da atividade, verifique a data da atividade antecessora."
			);
			break;
		}
		if( ( !verifica_pai( $arrCodigos[ $i ], $dt_ini, $dt_fim ) ) )
		{
			$erroData = array(
			'ptoid' => $arrCodigos[ $i ],
			'mensagem' => "Erro ao tentar modificar a data da atividade pai."
			);
			break;
		}
		if( !verifica_filhos( $arrCodigos[ $i ], $dt_ini, $dt_fim ) )
		{
			$erroData = array(
			'ptoid' => $arrCodigos[ $i ],
			'mensagem' => "Erro ao tentar alterar data de atividades filhas."
			);
			break;
		}
		if( !verifica_sucessor( $arrCodigos[ $i ], $dt_ini, $dt_fim ) )
		{
			$erroData = array(
			'ptoid' => $arrCodigos[ $i ],
			'mensagem' => "Erro ao tentar alterar data de atividades sucessoras."
			);
			break;
		}

		if( !altera_data_atividade( $arrCodigos[ $i ], $dt_ini, $dt_fim ) )
		{
			$erroData = array(
			'ptoid' => $arrCodigos[ $i ],
			'mensagem' => "Erro ao tentar alterar data da atividade.\\nVerifique se a atividade não está com as datas congeladas."
			);
			break;
		}
	}
	if( !$erroData )
	{
		ajusta_ordem( $ptotipo , $ptoordem , $ptoordem2 );
		$db->commit();
		unset( $_SESSION[ 'erroData' ] );
		unset( $_SESSION[ 'request' ] );
	}
	else
	{
		$sql =	' SELECT ' .
					'ptoid_pai' .
				' FROM ' .
					'monitora.planotrabalho' . 
				' WHERE ' . 
					'ptoid' . ' = ' . $ultimoId;
					
		$ptoid_pai = $db->pegaUm( $sql );
		
		$erroData[ "ptoid_pai" ] = $ptoid_pai ? $ptoid_pai : NULL;
		$_SESSION[ 'erroData' ] = $erroData;
		$_SESSION[ 'request' ] = $_REQUEST;
	}
}

function chamaCabecalho()
{
	global $db;
	
	$sql = ' SELECT ' .
				'pe.pjesnvisivel' . ' AS ' . 'visivel' . 
				',' . 
				'pe.pjesndatafechada' . ' AS ' . 'projfechado' . 
				',' .
				'pe.pjecod' . 
				',' . 
				'pe.pjedsc' . 
				',' . 
				'to_char(pe.pjedataini,\'dd/mm/YYYY\')' . ' AS ' . 'pjedataini' . 
				' , ' . 
				'to_char(pe.pjedatafim,\'dd/mm/YYYY\')' . ' AS ' . 'pjedatafim' . 
				' , ' . 
				'p.prodsc' . 
				',' .
				'pe.pjeprevistoano,pe.pjevlrano' .
			' FROM ' . 
				'monitora.projetoespecial pe' .
			' INNER JOIN ' .
				'produto p' .
			' ON ' .
				'p.procod' . ' = ' . 'pe.procod' .
			' WHERE ' .
				'pjeid' . ' = ' . $_SESSION['pjeid'];
				
	$res=$db->pegalinha($sql);
	if(is_array($res)) foreach($res as $k=>$v)
	{
		global ${$k};
		${$k}=$v;
	}
	$pjeinimt=0;
	$pjefimmt=0;
//	include_once( APPRAIZ."includes/cabecalho.inc" );
}
?>
