<?PHP
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );

if (! $_REQUEST['metid'])
	$_REQUEST['metid'] = 1;


if($_REQUEST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['requisicaoAjax']();
	exit;
}

if($_REQUEST['requisicao']){
	$_REQUEST['requisicao']();
	exit;
}


function montarGrafico(){

	//require_once ('jpgraph/jpgraph.php');
	//require_once ('jpgraph/jpgraph_line.php');
	include APPRAIZ . "includes/jpgraph/jpgraph.php";
	include APPRAIZ . "includes/jpgraph/jpgraph_line.php";

	// Some  data
	$ydata = explode(",", $_REQUEST['brasilCarregados']);
	$xdata = explode(",", $_REQUEST['anosCarregados']);

	if ($_REQUEST['regioes'])
	{
		$_REQUEST['regioes'] = str_replace("\\", "", $_REQUEST['regioes']);
		$regioes = explode(",", $_REQUEST['regioes']);
	}


	if ($_REQUEST['estados'])
		$estados = explode(",", $_REQUEST['estados']);

	if ($_REQUEST['mesoregioes'])
	{
		$_REQUEST['mesoregioes'] = str_replace("\\", "", $_REQUEST['mesoregioes']);
		$mesoregioes = explode(",", $_REQUEST['mesoregioes']);
	}

	if ($_REQUEST['municipios'])
	{
		$_REQUEST['municipios'] = str_replace("\\", "", $_REQUEST['municipios']);
		$municipios = explode(",", $_REQUEST['municipios']);
	}

	//$ydata = array(10,20);
	//$xdata = array(2010,2011);

	// Size of the overall graph
	$width=600;
	$height=250;

	// Create the graph and set a scale.
	// These two calls are always required
	$graph = new Graph($width,$height);
	$graph->SetScale('intlin');


	$periodo= $xdata[0].'-'.$xdata[sizeof($xdata)-1];
	// Setup margin and titles
	$graph->SetMargin(40,20,20,40);
	$graph->title->Set('Período');
	$graph->subtitle->Set($periodo);
	$graph->xaxis->title->Set('ANO');
	$graph->yaxis->title->Set('PNE');
	$graph->legend->SetFont(FF_FONT0,FS_NORMAL,10);
	$graph->legend->SetLineSpacing(5);
	$graph->legend->Pos(0.01,0.17);
	$graph->legend->SetFrameWeight(1);
	$graph->SetMargin(40, 150 ,0, 0);
	// Create the linear plot

	$lineplot=new LinePlot($ydata,$xdata);
	$lineplot ->SetWeight(3);
	$lineplot->SetColor('#0000ff');
	$lineplot->SetLegend('Brasil');

	// Add the plot to the graph
	$graph->Add($lineplot);



	global $db;

	$arrCores = array('#FF0000','#008000','#663300','#99CCCC','#FFA500','#6800FF','#003300','#CC00CC','#CCCCCC');
	$ctCores = 0;
	$ct = 0;


	if ($municipios)
	{
		foreach ($municipios as $mun)
		{
			$ct = $ct + 1;

			$ydata = array();

			$sql = "select round( pnevalor , 2) as pnevalor, mundescricao from pde.sase_pne p
				inner join territorios.municipio m on m.muncod = p.muncod
				where p.muncod= ".$mun." AND subid =" . $_REQUEST['subid'] .  " and pnetipo = 'M' order by pneano" ;


//			ver($sql,d);

			$arr = $db->carregar($sql);

			if ($arr)
			{
				foreach($arr as $p)
				{
					array_push ( $ydata, $p['pnevalor']);
				}
			}

			${'lineplot'.$ct}  = new  LinePlot ( $ydata,$xdata );
			${'lineplot'.$ct} ->SetColor($arrCores[$ctCores]);
			${'lineplot'.$ct} ->SetLegend($arr[0]['mundescricao']);
			${'lineplot'.$ct} ->SetWeight(3);
			$graph->Add( ${'lineplot'.$ct});

			$ctCores = $ctCores + 1;

			if ($ctCores == 9 )
				$ctCores = 0;
		}
	}


	if ($estados)
	{
		foreach ($estados as $uf)
		{
			$ct = $ct + 1;

			$ydata = array();

			$sql = "select round( pnevalor , 2) as pnevalor, estuf from pde.sase_pne where estuf= '".$uf."'  AND subid =". $_REQUEST['subid'] . " and pnetipo = 'E' order by pneano";

			$arr = $db->carregar($sql);

			if ($arr)
			{
				foreach($arr as $p)
				{
					array_push ( $ydata, $p['pnevalor']);
				}
			}
			${'lineplot'.$ct}  = new  LinePlot ( $ydata,$xdata );
			${'lineplot'.$ct} ->SetColor($arrCores[$ctCores]);
			${'lineplot'.$ct} ->SetLegend($arr[0]['estuf']);
			${'lineplot'.$ct} ->SetWeight(3);
			$graph->Add( ${'lineplot'.$ct});

			$ctCores = $ctCores + 1;

			if ($ctCores == 9 )
				$ctCores = 0;

		}
	}

	if ($regioes)
	{
		foreach ($regioes as $reg)
		{
			$ct = $ct + 1;

			$ydata = array();

			$sql = "select round( pnevalor , 2) as pnevalor, regdescricao from pde.sase_pne p
					inner join territorios.regiao r on p.regcod = r.regcod
					where p.regcod= '$reg' AND subid =" . $_REQUEST['subid'] . " and pnetipo = 'R' order by pneano" ;


			$arr = $db->carregar($sql);

			if ($arr)
			{
				foreach($arr as $p)
				{
					array_push ( $ydata, $p['pnevalor']);
				}
			}
			${'lineplot'.$ct}  = new  LinePlot ( $ydata,$xdata );
			${'lineplot'.$ct} ->SetColor($arrCores[$ctCores]);
			${'lineplot'.$ct} ->SetLegend($arr[0]['regdescricao']);
			${'lineplot'.$ct} ->SetWeight(3);
			$graph->Add( ${'lineplot'.$ct});

			$ctCores = $ctCores + 1;

			if ($ctCores == 9 )
				$ctCores = 0;

		}
	}

	if ($mesoregioes)
	{
		foreach ($mesoregioes as $mes)
		{
			$ct = $ct + 1;

			$ydata = array();

			$sql = "select round( pnevalor , 2) as pnevalor, mesdsc from pde.sase_pne p
			inner join territorios.mesoregiao m on p.mescod = m.mescod
			where p.mescod= '$mes' AND subid =" . $_REQUEST['subid'] . " and pnetipo = 'S' order by pneano" ;


			$arr = $db->carregar($sql);

			if ($arr)
			{
				foreach($arr as $p)
				{
					array_push ( $ydata, $p['pnevalor']);
				}
			}
			${'lineplot'.$ct}  = new  LinePlot ( $ydata,$xdata );
			${'lineplot'.$ct} ->SetColor($arrCores[$ctCores]);
			${'lineplot'.$ct} ->SetLegend($arr[0]['mesdsc']);
			${'lineplot'.$ct} ->SetWeight(3);
			$graph->Add( ${'lineplot'.$ct});

			$ctCores = $ctCores + 1;

			if ($ctCores == 9 )
				$ctCores = 0;

		}
	}

	$graph->Stroke();
}


function listarDadosMunicipios($municipios, $subid)
{

	global $db;

	foreach ($municipios as $mun)
	{
		$sql = "select '<td>'||round( pnevalor , 2)||'</td>' as pnevalor, mundescricao from pde.sase_pne p
				inner join territorios.municipio m on m.muncod = p.muncod
				where p.muncod= ".$mun." AND subid =". $subid . " and pnetipo = 'M' order by pneano" ;

		$arr = $db->carregar($sql);

		if ($arr)
		{
			echo "<tr><td>".$arr[0]['mundescricao']."</td>";
			foreach($arr as $p)
			{
				$p['pnevalor'] = str_replace(".",",",$p['pnevalor']);

				echo $p['pnevalor'];
			}
			echo"</tr>";
		}
		else
		{
			return '';
		}
	}

	$sql =  "select estuf from territorios.municipio where muncod in (".implode(",", $municipios).") group by estuf order by 1";
	$arr = $db->carregarColuna($sql);

	return implode(",",$arr);


}

function listarDadosRegioes($regioes, $subid)
{
	global $db;

	$arrIDEstadosCarregado = array();

	foreach ($regioes as $r)
	{
		$sql = "select pneid,   '<td>'||round( pnevalor , 2)||'</td>' as pnevalor, round( pnevalor , 2) as pnevalorori, regdescricao from pde.sase_pne p
				inner join territorios.regiao r on p.regcod = p.regcod
				where r.regcod= '$r'  AND subid = $subid and pnetipo = 'R' order by pneano";

		$arr = $db->carregar($sql);

		if ($arr)
		{
			echo "<tr><td>".$arr[0]['regdescricao']."</td>";
			foreach($arr as $p)
			{
				$p['pnevalor'] = str_replace(".",",",$p['pnevalor']);

				echo $p['pnevalor'];

				//array_push ( $arrIDEstadosCarregado, $p['pneid']);
			}
			echo"</tr>";
		}
	}

	return $arrIDEstadosCarregado;
}

function listarDadosMesoregioes($mesoregioes, $subid)
{
	global $db;


	foreach ($mesoregioes as $m)
	{
		$sql = "select pneid,   '<td>'||round( pnevalor , 2)||'</td>' as pnevalor, round( pnevalor , 2) as pnevalorori, mesdsc from pde.sase_pne p
		inner join territorios.mesoregiao m on p.mescod = m.mescod
		where p.mescod= '$m'  AND subid = $subid and pnetipo = 'S' order by pneano";

		$arr = $db->carregar($sql);

		if ($arr)
		{
			echo "<tr><td>".$arr[0]['mesdsc']."</td>";
			foreach($arr as $p)
			{
				$p['pnevalor'] = str_replace(".",",",$p['pnevalor']);

				echo $p['pnevalor'];

				//array_push ( $arrIDEstadosCarregado, $p['pneid']);
			}
			echo"</tr>";
		}
	}
}



function listarDadosEstados($estados, $subid)
{
	global $db;

	$arrIDEstadosCarregado = array();

	foreach ($estados as $uf)
	{
		$sql = "select pneid,   '<td>'||round( pnevalor , 2)||'</td>' as pnevalor, round( pnevalor , 2) as pnevalorori, estuf from pde.sase_pne where estuf= '".$uf."'  AND subid =". $subid . " and pnetipo = 'E' order by pneano";

		$arr = $db->carregar($sql);

		if ($arr)
		{
			echo "<tr><td>".$uf."</td>";
			foreach($arr as $p)
			{
				$p['pnevalor'] = str_replace(".",",",$p['pnevalor']);

				echo $p['pnevalor'];

				array_push ( $arrIDEstadosCarregado, $p['pneid']);
			}
			echo"</tr>";
		}
	}

	return $arrIDEstadosCarregado;
}



function listagemPrincipal()
{
	global $db;

	$sql = "select subid, subtitulo from pde.sase_submeta where substatus = 'A' and metid = ".$_REQUEST['metid'];
	$arr = $db->carregar($sql);

	$regioes = trim( $_REQUEST['regioes'], ',');
	$regioes = str_replace("\'", '',$regioes);

	$estados = trim( $_REQUEST['estados'], ',');
	$estados = str_replace("\'", '',$estados);

	$mesoregioes = trim( $_REQUEST['mesoregioes'], ',');
	$mesoregioes = str_replace("\'", '',$mesoregioes);

	$municipios = trim( $_REQUEST['municipios'], ',');
	$municipios = str_replace("\\", '',$municipios);

	foreach($arr as $s):
	?>
				<table class="tabela_box_azul_escuro" cellpadding="2" cellspacing="1" width="100%" >
					<tr>
						<td><?php echo $s['subtitulo']; ?></td>
					</tr>
					<tr>
						<td class="center bold" >
							<table class="tabela_box_azul_escuro" border ='1' cellpadding="2" cellspacing="1" width="100%" >
								<?php
								$arrbrasilCarregados = array();
								$arranosCarregados = array();


								$sql = "select '<td>'||round( pnevalor , 2)||'</td>' as pnevalor, '<td>'||pneano||'</td>' as pneano , round( pnevalor , 2) as pnevalorori, pneano as pneanoori  from pde.sase_pne where estuf is null  AND subid =". $s['subid'] . " and pnetipo = 'E' order by pneano" ;
								$arr = $db->carregar($sql);

								echo '<tr><td></td>';
								foreach($arr as $p)
								{
									echo $p['pneano'];
									array_push($arranosCarregados, $p['pneanoori']);
								}
								echo '</tr><tr><td>Brasil</td>';

								foreach($arr as $p)
								{
									$p['pnevalor'] = str_replace(".",",",$p['pnevalor']);

									echo $p['pnevalor'];
									array_push($arrbrasilCarregados, $p['pnevalorori']);
								}
								echo '</tr>';

								if ($regioes)
								{
									$arrRegioes = explode(',',$regioes);

									listarDadosRegioes($arrRegioes, $s['subid']);
								}

								if ($estados)
								{
									$arrEstados = explode(',',$estados);
									listarDadosEstados($arrEstados, $s['subid']);
								}

								if ($mesoregioes)
								{
									$arrMesoregioes = explode(',',$mesoregioes);
									listarDadosMesoregioes($arrMesoregioes, $s['subid']);
								}

								if ($municipios)
								{
									$arrMunicipios = explode(',',$municipios);

									listarDadosMunicipios($arrMunicipios, $s['subid']);
								}

								?>
							</table>
						</td>
					</tr>
					<tr>
						<td class="tabela_painel">
							<table class="tabela_painel" cellpadding="2" cellspacing="1" width="100%" >
								<tr>
									<td align=center>
									<?php
                                    $where = '';
									if ($_REQUEST['estados']) {
									    $where .= " OR (pne.estuf IN (" . substr(str_replace('\\', '', $_REQUEST['estados']), 0, -1) . ") and pnetipo = 'E') ";
									}
									if ($_REQUEST['municipios']) {
									    $where .= " OR (pne.muncod IN (" . $municipios . ") and pnetipo = 'M') ";
									}
									if ($_REQUEST['regioes']) {
									    $where .= " OR (pne.regcod IN (" . substr(str_replace('\\', '', $_REQUEST['regioes']), 0, -1) . ") and pnetipo = 'R') ";
									}
									if ($_REQUEST['mesoregioes']) {
									    $where .= " OR (pne.mescod IN (" . substr(str_replace('\\', '', $_REQUEST['mesoregioes']), 0, -1) . ") and pnetipo = 'S') ";
									}

                                    $sql = "select
                                                case
                                                    when pnetipo = 'M' then mundescricao
                                                    when pnetipo = 'R' then regdescricao
                                                    when pnetipo = 'S' then mesdsc
                                                    else coalesce(pne.estuf, '1Brasil')
                                                end as descricao
                                                , round(pnevalor, 2) as pnevalor, pneano, pnetipo, pne.subid, sub.metid, sub.subtitulo, *
                                            from pde.sase_pne  pne
                                                inner join pde.sase_submeta sub on sub.subid = pne.subid
                                                left join territorios.municipio m on m.muncod = pne.muncod
                                                left join territorios.regiao r on r.regcod = pne.regcod
                                                left join territorios.mesoregiao mr on mr.mescod = pne.mescod
                                            where sub.subid = {$s['subid']}
                                            AND (
                                                (pne.estuf is null and pnetipo = 'E')
                                                $where
                                            )
                                            order by pne.subid, pne.pneano, pnetipo, descricao";

//								   ver($sql);

                                    $dados = $db->carregar($sql);
                                    $anos = array();
                                    $dadosAgrupados = array();
                                    foreach ($dados as $dado) {
                                        $dadosAgrupados[str_replace('1Brasil', 'Brasil', tirar_acentos($dado['descricao']))][] = (float) $dado['pnevalor'];
                                        $anos[$dado['pneano']] = $dado['pneano'];
                                    }

                                    $aDados = array();
                                    foreach ($dadosAgrupados as $descricao => $aDado) {
                                        $aDados[] = array('name' => $descricao, 'data'=>$aDado);
                                    }

                                    echo '<div style="margin-bottom: 10px;">' . geraGraficoLinha($aDados, array_values($anos), "grafico_" . $s['subid'], '',200) . '</div>';
									?>
									</td>
								</tr>
							</table>
						</td>
					</tr>
				</table>
				<br>
				<?php endforeach;
}


function criarAbasMetasPNE()
{
	global $db;

	$sql = "select metid, mettitulo from pde.sase_meta where metstatus = 'A' order by metid ";
	$arrMetas = $db->carregar($sql);

	$abas = array();

	foreach($arrMetas as $meta)
	{
		$arrMeta = array("descricao" =>  'Meta '.$meta['metid'], "meta" => $meta['metid'], "extenso" => $meta['mettitulo']);

		array_push($abas, $arrMeta);

	}

	return $abas;
}



function listarEstados()
{
	$sql = " Select	estuf AS codigo, estdescricao AS descricao From territorios.estado ";

	$regioes =   $_REQUEST['regioes'];

	if ($regioes)
	{
		$regioes =  str_replace("\\", "", trim($regioes, ',')) ;

		$sql .= "where regcod in(".$regioes.")";
	}

	$estados =   $_REQUEST['estados'];

	if ($estados)
	{
		$estados =  str_replace("\\", "", trim($estados, ',')) ;

		$sqlselecionados = "select estuf as codigo, estdescricao as descricao from territorios.estado where estuf in ($estados) ";

		if ($regioes)
		{
			$sqlselecionados .= " and regcod in ($regioes)";
		}
	}


	mostrarComboPopupLocal( 'Estado', 'slEstado',  $sql,$sqlselecionados, 'Selecione os Estados', null,'atualizarRelacionadosRegiao(2)',false);
}

function listarMesoregioes()
{
	$sql = " Select	mescod AS codigo, mesdsc AS descricao From territorios.mesoregiao ";

	$regioes =   $_REQUEST['regioes'];

	if ($regioes)
	{
		$regioes =  str_replace("\\", "", trim($regioes, ',')) ;

		$sql .= "where estuf in (select estuf from territorios.estado where regcod in ($regioes))";
	}

	$estados =   $_REQUEST['estados'];

	if ($estados)
	{
		$estados =  str_replace("\\", "", trim($estados, ',')) ;

		if ($regioes)
			$sql .= "and estuf in(".$estados.")";
		else
			$sql .= "where estuf in(".$estados.")";

	}


	mostrarComboPopupLocal( 'Mesorregião', 'slMesoregiao',  $sql,$sqlselecionados, 'Selecione os Estados', null,'atualizarRelacionadosRegiao(3)',false);
}

function listarMunicipios()
{
	$sql = " Select	muncod AS codigo, mundescricao AS descricao From territorios.municipio ";


	$regioes =   $_REQUEST['regioes'];

	if ($regioes)
	{
		$regioes =  str_replace("\\", "", trim($regioes, ',')) ;

		$sql .= "where estuf in (select estuf from territorios.estado where regcod in ($regioes))";
	}


	$estados =   $_REQUEST['estados'];

	if ($estados)
	{
		$estados =  str_replace("\\", "", trim($estados, ',')) ;

		if ($regioes)
			$sql .= "and estuf in(".$estados.")";
		else
			$sql .= "where estuf in(".$estados.")";
	}


	$municipios =   $_REQUEST['municipios'];

	if ($municipios)
	{
		$municipios =  str_replace("\\", "", trim($municipios, ',')) ;

		$sqlselecionados = "select muncod as codigo, mundescricao as descricao from territorios.municipio where muncod in ($municipios) ";

		if ($estados)
		{
			$sqlselecionados .= " and estuf in ($estados)";
		}
	}

	mostrarComboPopupLocal( 'Município', 'slMunicipio',  $sql, $sqlselecionados, 'Selecione os Municipios', null, 'atualizarRelacionadosRegiao(4)' ,false);
}


function montarAbasArrayLocal($itensMenu, $url = false, $boOpenWin = false)
{
	$url = $url ? $url : $_SERVER['REQUEST_URI'];

	if (is_array($itensMenu)) {
		$rs = $itensMenu;
	} else {
		global $db;
		$rs = $db->carregar($itensMenu);
	}

	$menu    = '<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" class="notprint">'
			. '<tr>'
					. '<td>'
							. '<table cellpadding="0" cellspacing="0" align="left">'
									. '<tr>';

	$nlinhas = count($rs) - 1;

	for ($j = 0; $j <= $nlinhas; $j++) {
		extract($rs[$j]);

		if ($url != $meta && $j == 0)
			$gifaba = 'aba_nosel_ini.gif';
		elseif ($url == $meta && $j == 0)
		$gifaba = 'aba_esq_sel_ini.gif';
		elseif ($gifaba == 'aba_esq_sel_ini.gif' || $gifaba == 'aba_esq_sel.gif')
		$gifaba = 'aba_dir_sel.gif';
		elseif ($url != $meta)
		$gifaba = 'aba_nosel.gif';
		elseif ($url == $meta)
		$gifaba = 'aba_esq_sel.gif';

		if ($url == $meta) {
			$giffundo_aba = 'aba_fundo_sel.gif';
			$cor_fonteaba = '#000055';
		} else {
			$giffundo_aba = 'aba_fundo_nosel.gif';
			$cor_fonteaba = '#4488cc';
		}

		$menu .= '<td height="20" valign="top"><img id="abameta'.$meta.'" src="../imagens/'.$gifaba.'" width="11" height="20" border="0"></td>'
				. '<td height="20" title="'.$extenso.'"  id="abametaf'.$meta.'" align="center" valign="middle" background="../imagens/'.$giffundo_aba.'" style="color:'.$cor_fonteaba.'; padding-left: 10px; padding-right: 10px;cursor:pointer;" onclick="selecionaAba('.$meta.');$(\'#metid\').val('.$meta.');listarSubmetas(pegaSelecionados(\'slRegiao[]\'), pegaSelecionados(\'slEstado[]\'),pegaSelecionados(\'slMesoregiao[]\'),pegaSelecionados(\'slMunicipio[]\'))">';

		if ($meta != $url) {
			$menu .= $descricao;
		} else {
			$menu .= $descricao . '</td>';
		}
	}

	if ($gifaba == 'aba_esq_sel_ini.gif' || $gifaba == 'aba_esq_sel.gif')
		$gifaba = 'aba_dir_sel_fim.gif';
	else
		$gifaba = 'aba_nosel_fim.gif';

	$menu .= '<td height="20" valign="top"><img src="../imagens/'.$gifaba.'" width="11" height="20" alt="" border="0"></td></tr></table></td></tr></table>';

	return $menu;
}

function mostrarComboPopupLocal( $stDescricao, $stNomeCampo, $sql_combo, $sql_carregados, $stTextoSelecao, Array $where=null, $funcaoJS=null, $semTR=false, $intervalo=false , $arrVisivel = null , $arrOrdem = null, $obrig = false, $campoContem = true ){
	global $db, $$stNomeCampo;
	if ( $_REQUEST[$stNomeCampo] && $_REQUEST[$stNomeCampo][0] && !empty( $sql_carregados ) ) {
		if(!is_array($_REQUEST[$stNomeCampo])){
			$_REQUEST[$stNomeCampo][0] = $_REQUEST[$stNomeCampo];
		}
		$sql_carregados = sprintf( $sql_carregados, "'" . implode( "','", $_REQUEST[$stNomeCampo] ) . "'" );
		$$stNomeCampo = $db->carregar( sprintf( $sql_combo, $sql_carregados ) );
	}
	if( !empty($sql_carregados) ){
		$$stNomeCampo = $db->carregar($sql_carregados);
	}

	if(!$semTR)
	{
		echo '<tr id="tr_'.$stNomeCampo.'">';
	}

	echo '<td width="10%" valign="top" onclick="javascript:onOffCampo( \'' . $stNomeCampo . '\' );">
			' . $stDescricao . '
			<input type="hidden" id="' . $stNomeCampo . '_campo_flag" name="' . $stNomeCampo . '_campo_flag" value="' . ( empty( $$stNomeCampo ) ? '0' : '1' ) . '"/>
		</td>
		<td>';

	echo '<div id="' . $stNomeCampo . '_campo_off" style="color:#a0a0a0;';
	echo !empty( $$stNomeCampo ) ? 'display:none;' : '';
	echo '" onclick="javascript:onOffCampo( \'' . $stNomeCampo . '\' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>';
	echo '<div id="' . $stNomeCampo . '_campo_on" ';
	echo empty( $$stNomeCampo ) ? 'style="display:none;"' : '';
	echo '>';
	combo_popupLocal( $stNomeCampo, sprintf( $sql_combo, '' ), $stTextoSelecao, '400x400', 0, array(), '', 'S', false, false, 10, 400, null, null, '', $where, null, true, false, $funcaoJS, $intervalo , $arrVisivel, $arrOrdem);

	if( $obrig )
	{
		echo '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">';
	}

	echo '</div>
			</td>';

	if(!$semTR)	echo '</tr>';
}


function combo_popupLocal( $nome, $sql, $titulo, $tamanho_janela = '400x400', $maximo_itens = 0,
		$codigos_fixos = array(), $mensagem_fixo = '', $habilitado = 'S', $campo_busca_codigo = false,
		$campo_flag_contem = false, $size = 10, $width = 400 , $onpop = null, $onpush = null, $param_conexao = false, $where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false, $funcaoJS=null, $intervalo=false, $arrVisivel = null , $arrOrdem = null)
{

	global ${$nome};
	unset($dados_sessao);
	// prepara parametros
	$maximo_itens = abs( (integer) $maximo_itens );
	$codigos_fixos = $codigos_fixos ? $codigos_fixos : array();
	// prepara sessão
	$dados_sessao = array(
			'sql' => (string) $sql, // o sql é armazenado para ser executado posteriormente pela janela popup
			'titulo' => $titulo,
			'indice' => $indice_visivel,
			'maximo' => $maximo_itens,
			'codigos_fixos' => $codigos_fixos,
			'mensagem_fixo' => $mensagem_fixo,
			'param_conexao' => $param_conexao,
			'where'			=> $where,
			'mostraPesquisa'=> $mostraPesquisa,
			'intervalo'     => $intervalo,
			'arrVisivel'    => $arrVisivel,
			'arrOrdem'     => $arrOrdem
	);

	if ( !isset( $_SESSION['indice_sessao_combo_popup'] ) )
	{
		$_SESSION['indice_sessao_combo_popup'] = array();
	}
	unset($_SESSION['indice_sessao_combo_popup'][$nome]);
	$_SESSION['indice_sessao_combo_popup'][$nome] = $dados_sessao;

	// monta html para formulario
	$tamanho    = explode( 'x', $tamanho_janela );
	$onclick    = ' onclick="javascript:combo_popup_alterar_campo_busca( this );" ';

	/*** Adiciona a função Javascript ***/
	$funcaoJS = (is_null($funcaoJS)) ? 'false' : "'" . $funcaoJS . "'";

	$ondblclick = ' ondblclick="javascript:combo_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ', '.$funcaoJS.' );" ';
	$ondelete   = ' onkeydown="javascript:combo_popup_remove_selecionados( event, \'' . $nome . '\' );" ';
	$onpop		= ( $onpop == null ) ? $onpop = '' : ' onpop="' . $onpop . '"';
	$onpush		= ( $onpush == null ) ? $onpush = '' : ' onpush="' . $onpush . '"';
	$habilitado_select = $habilitado == 'S' ? '' : ' disabled="disabled" ' ;
	$select =
	'<select ' .
	'maximo="'. $maximo_itens .'" tipo="combo_popup" ' .
	'multiple="multiple" size="' . $size . '" ' .
	'name="' . $nome . '[]" id="' . $nome . '" '.
	$onclick . $ondblclick . $ondelete . $onpop . $onpush  .
	'class="CampoEstilo" style="width:' . $width . 'px;" ' .
	$habilitado_select .
	'>';

	if($value && count( $value ) > 0){
		$itens_criados = 0;
		foreach ( $value as $item )
		{
			$select .= '<option value="' . $item['codigo'] . '">' . simec_htmlentities( $item['descricao'] ) . '</option>';
			$itens_criados++;
			if ( $maximo_itens != 0 && $itens_criados >= $maximo_itens )
			{
				break;
			}
		}
	} elseif ( ${$nome} && count( ${$nome} ) > 0 ) {
		$itens_criados = 0;
		if( is_array(${$nome}) ){
			foreach ( ${$nome} as $item )
			{
				$select .= '<option value="' . $item['codigo'] . '">' . simec_htmlentities( $item['descricao'] ) . '</option>';
				$itens_criados++;
				if ( $maximo_itens != 0 && $itens_criados >= $maximo_itens )
				{
					break;
				}
			}
		}
	}
	else if ( $habilitado == 'S' )
	{
		$select .= '<option value="">Duplo clique para selecionar da lista</option>';
	}
	else
	{
		$select .= '<option value="">Nenhum</option>';
	}
	$select .= '</select>';
	$buscaCodigo = '';

	#Alteração feita por wesley romualdo
	#caso a consulta não seja por descrição e sim por codigo, não permitir digitar string no campo de consulta.
	if($campo_busca_descricao == true ){
	$paramentro = "";
	$complOnblur = "";
	} else {
	$paramentro = "onkeyup=\"this.value=mascaraglobal('[#]',this.value);\"";
			$complOnblur = "this.value=mascaraglobal('[#]',this.value);";
	}

	if ( $campo_busca_codigo == true && $habilitado == 'S' )
	{
		$buscaCodigo .= '<input type="text" id="combopopup_campo_busca_' . $nome . '" onkeypress="combo_popup_keypress_buscar_codigo( event, \'' . $nome . '\', this.value );" '.$paramentro.' onmouseover="MouseOver( this );" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this); '.$complOnblur.'" class="normal" style="margin: 2px 0;" />';
		$buscaCodigo .= '&nbsp;<img title="adicionar" align="absmiddle" src="/imagens/check_p.gif" onclick="combo_popup_buscar_codigo( \'' . $nome . '\', document.getElementById( \'combopopup_campo_busca_' . $nome . '\' ).value );"/>';
		$buscaCodigo .= '&nbsp;<img title="remover" align="absmiddle" src="/imagens/exclui_p.gif" onclick="combo_popup_remover_item( \'' . $nome . '\', document.getElementById( \'combopopup_campo_busca_' . $nome . '\' ).value, true );"/>';
		$buscaCodigo .= '&nbsp;<img title="abrir lista" align="absmiddle" src="/imagens/pop_p.gif" onclick="combo_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ' );"/>';
		$buscaCodigo .= '<br/>';
	}
	#Fim da alteração realizada por wesley romualdo

	$flagContem = '';
	if ( $campo_flag_contem == true )
	{
	$nomeFlagContemGlobal = $nome . '_campo_excludente';
		global ${$nomeFlagContemGlobal};
		$flagContem .= '<input type="checkbox" id="' . $nome . '_campo_excludente" name="' . $nome . '_campo_excludente" value="1" ' . ( ${$nomeFlagContemGlobal} ? 'checked="checked"' : '' ) . ' style="margin:0;" />';
		$flagContem .= '&nbsp;<label for="' . $nome . '_campo_excludente">Não contém</label>';
	}
				$cabecalho = '';
				if ( $buscaCodigo != '' || $flagContem != '' )
				{
					$cabecalho .= '<table width="' . $width . '" border="0" cellspacing="0" cellpadding="0"><tr>';
					$cabecalho .= '<td align="left">' . $buscaCodigo . '</td>';
					$cabecalho .= '<td align="right">' . $flagContem . '</td>';
					$cabecalho .= '</tr></table>';
}
	print $cabecalho . $select  . ' <img src="../imagens/pop_p.gif" style="cursor:pointer;" align="absmiddle" onclick="javascript:combo_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ', '.$funcaoJS.' );">';
}



?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="X-UA-Compatible" content="IE=9" />
<meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">
<title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>
<body>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/jquery-jqplot-1.0.0/jquery.jqplot.min.js"></script>
<script type="text/javascript" src="../includes/jquery-jqplot-1.0.0/plugins/jqplot.pieRenderer.min.js"></script>
<script type="text/javascript" src="../includes/jquery-jqplot-1.0.0/plugins/jqplot.donutRenderer.min.js"></script>

<script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>

<script language="javascript" type="text/javascript" src="js/estrategico.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<link rel="stylesheet" type="text/css" href="../includes/jquery-jqplot-1.0.0/jquery.jqplot.min.css" />
 <style>
  	.fundo_td{background-color:#0F6D39}
  	.titulo_pagina{font-weight:bold;font-size:20px;color:#FFFFFF}
  	.titulo_box{font-weight:bold;font-size:18px;color:#FFFFFF;margin-top:15px;text-shadow:#000000 0px 1px 2px}
  	.subtitulo_box{font-weight:normal;font-size:10px;color:#FFFFFF}
  	.fundo_td:hover {background-color:#0D8845}
  	.fundo_td{text-align:left;vertical-align:top;}
  	.tabela_painel{font-weight:bold;font-size:8px;color:#FFFFFF;font-family:fantasy}
  	.lista_metas{float:left}
  	#busca{background: none repeat scroll 0% 0% rgb(255, 255, 255); width:400px;border-width: 1px; border-style: solid; border-color: rgb(204, 204, 204) rgb(153, 153, 153) rgb(153, 153, 153) rgb(204, 204, 204); color: rgb(0, 0, 0); font: 18px arial,sans-serif bold; height: 35px;}
  	.tabela_box{color:#FFFFFF;}
  	.tabela_box td{background-color:#3CB371;text-shadow:#000000 0px 2px 2px}
  	.tabela_box_azul td{background-color:#63B8FF;text-shadow:#000000 0px 2px 2px;color:#FFFFFF;}
  	.tabela_box_azul_escuro td{background-color:#204481;text-shadow:#000000 0px 2px 2px;color:#FFFFFF;}
  	.fundo_td_azul{background-color:#2B86EE}
  	.fundo_td_azul:hover{background-color:#01A2D8}
  	.fundo_td_verde{background-color:#0F6D39}
  	.fundo_td_verde:hover{background-color:#32CD32}
   	.fundo_td_laranja{background-color:#EE9200}
  	.fundo_td_laranja:hover{background-color:#EBB513}
  	.fundo_td_vermelho{background-color:#BB0000}
  	.fundo_td_vermelho:hover{background-color:#DD0000}
  	.fundo_td_roxo{background-color:#5333AD}
  	.fundo_td_roxo:hover{background-color:#6A5ACD}
  	.fundo_td_azul_escuro{background-color:#152D56}
  	.fundo_td_azul_escuro:hover{background-color:#1F3864}
  	.div_fotos{background-color:#204481;cursor:pointer;margin-bottom:3px;text-shadow:#000000 0px 1px 2px;width:450px;margin-bottom:2px}
  	 body{background-image:url('../imagens/degrade-fundo-preto.png');background-repeat:repeat-x;background-color:#330000;margin:0px;padding-top:0px;}
  	.fundo_titulo{background-image:url('../imagens/cockpit/fundo_enem.jpg');background-repeat:repeat-x;background-position:2px -50px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
  	.numero{text-align:right;font-size:11px;font-family:verdana}
  	.center{text-align:center}
  	.titulo_box a{color:#FFFFFF;text-decoration:none;}
  	.titulo_box a:hover{color:#FFFFFF;text-decoration:none;}
  	.div_fotos_interno{width:98%;height:85px}
  	.bold{font-weight:bold}
  	.esquerda{text-align:left;}
  	.link{cursor:pointer}
 </style>
<style>#mask{position:absolute;left:0;top:0;z-index:9000;background-color:#000;display:none;}#boxes .window{position:fixed;left:0;top:0;width:440px;height:200px;display:none;z-index:9999;padding:20px;}#boxes #dialog{width:375px;height:300px;padding:10px;background-color:#ffffff;}</style>
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="titulo_pagina" >
		<div style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A';">
			<img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;" />
			<div style="float:left" class="titulo_box" ><?php echo SIGLA_SISTEMA; ?><br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
		</div>
		<img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
		</td>
	</tr>
</table>

<script>



function selecionaAba(metaaba)
{
	 $('[id^="abametaf"]').css("color","#4488cc");
	 $('[id^="abametaf"]').css("font-weight","");

	 $('#abametaf'+metaaba).css("font-weight","bold");
	 $('#abametaf'+metaaba).css("color","navy");
}


function onOffCampo( campo )
{
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

function buscar(busca) {
	window.open('/painel/painel.php?modulo=principal/painel_controle&acao=A&buscacockpit='+busca,'Observações','scrollbars=yes,height=800,width=1500,status=no,toolbar=no,menubar=no,location=no');
}

function pegaSelecionados(elemento)
{
	var result = '';
	var elemento = document.getElementsByName(elemento)[0];

	for (var i=0; i<elemento.options.length; i++){
		if (elemento.options[i].value != '')
		{
			result += "'"+elemento.options[i].value + "',";
		}
	}

	return result;
}


function selecionaSubmeta(metid)
{
	this.formulario.metid.value = metid;
	this.formulario.submit();
}

function atualizarRelacionadosRegiao(requisicao) {
	//alert(requisicao);

	//1-estado(chamado pela lista de regiao),2-mesoregiao(chamado pela lista de estado), 3-municipio (chamado pela lista de mesoregiao)
	if (requisicao == 1)
	{
		requisicaoAjax = 'listarEstados';
	}
	else if (requisicao == 2)
	{
		requisicaoAjax = 'listarMesoregioes';
	}
	else if (requisicao == 3)
	{
		requisicaoAjax = 'listarMunicipios';
	}

	jQuery.ajax({
		type: "POST",
		url: window.location,
		data: "requisicaoAjax="+requisicaoAjax+"&regioes="+pegaSelecionados('slRegiao[]')+"&estados="+pegaSelecionados('slEstado[]')+"&mesoregioes="+pegaSelecionados('slMesoregiao[]')+"&municipios="+pegaSelecionados('slMunicipio[]'),
		success: function(retorno){
			if (requisicao == 1)
			{
				$('#tabelaEstados').html(retorno);
				atualizarRelacionadosRegiao(2);
			}
			else if (requisicao == 2)
			{
				$('#tabelaMesoregioes').html(retorno);
				atualizarRelacionadosRegiao(3);
			}
			else if (requisicao == 3)
			{
				$('#tabelaMunicipios').html(retorno);
			}

			if (requisicao == 3 || requisicao == 4)
				listarSubmetas(pegaSelecionados('slRegiao[]'),pegaSelecionados('slEstado[]'),pegaSelecionados('slMesoregiao[]'),pegaSelecionados('slMunicipio[]'));

		}
	});
}

function listarSubmetas(regioes, estados, mesoregioes, municipios)
{
	jQuery.ajax({
		type: "POST",
		url: window.location,
		data: "requisicaoAjax=listagemPrincipal&metid="+$('#metid').val()+"&regioes="+regioes+"&estados="+estados+"&mesoregioes="+mesoregioes+"&municipios="+municipios,
		success: function(retorno){
				$('#divListagem').html(retorno);
		}
	});
}


 $(document).ready(function() {
   listarSubmetas('','','','');
   selecionaAba(1);
 });

</script>
<form name='formulario' action='estrategico.php?modulo=principal/cockpit_pne&acao=A' method='post'  >
<input type='hidden' name='metid' value = ''  id = "metid" >
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr >
		<td class="fundo_titulo" style="text-align:center" colspan="2" ><div style="margin:28px" >PNE</div></td>
	</tr>
	<TR>
		<TD colspan=2>
			<?php
			 echo montarAbasArrayLocal( criarAbasMetasPNE() , "par.php?modulo=principal/listaMunicipios&acao=A" );
			?>
		</TD>
	</TR>
	<tr>
		<td class="fundo_td" valign=top width="35%" >
			<div>
                <img style="float:left" src="../imagens/icones/icons/doc.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
					Pesquisa<br/>
                </div>
            </div>
			<table  cellpadding="5" cellspacing="1" width="100%">
				<?php
				#Região
				$sql = " Select	regcod AS codigo, regdescricao AS descricao From territorios.regiao order by regdescricao";

				mostrarComboPopupLocal( 'Região', 'slRegiao',  $sql, "", 'Selecione as Regiões', null,'atualizarRelacionadosRegiao(1)',false);
				?>
			</table>
			<table  cellpadding="5" cellspacing="1" width="100%" id = "tabelaEstados" class="filtro_combo">
				<?php
				listarEstados();
				?>
			</table>
			<table  cellpadding="5" cellspacing="1" width="100%" id = "tabelaMesoregioes" class="filtro_combo">
				<?php
				listarMesoregioes();
				?>
			</table>
			<table  cellpadding="5" cellspacing="1" width="100%" id = "tabelaMunicipios" class="filtro_combo">
				<?php
				listarMunicipios();
				?>
			</table>
		</td>
        <td class="fundo_td_azul_escuro" valign="top">
            <div>
                <img style="float:left" src="../imagens/icones/icons/mundo.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >
                    <a href="#" >Dados PNE<br/></a>
                </div>
            </div>
            <table width= 100%>
                <tr>
                    <td id="divListagem">
                    </td>
                </tr>
            </table>
        </td>
	</tr>
	<tr>
		<td class="fundo_td_laranja" colspan="2">
		    <div style="text-align:center;"  >
		    <img src="../imagens/icones/icons/executiverel.png"  style="vertical-align:middle;"  />
		    <input type="text" onclick="this.style.color='#000000';this.value='';"  name="busca" size="61" maxlength="60" value="Digite aqui o que você procura" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);if(this.value==''){this.style.color='#D3D3D3';this.value='Digite aqui o que você procura'}" id='busca' onkeyup='exibeBuscaRegionalizacaoEnter(event)' style='color:#D3D3D3;'    title='' class=' normal' />
		    <img src="../imagens/icones/icons/Find.png"  style="vertical-align:middle;width:35px;height:35px;cursor:pointer;" onclick="buscar(document.getElementById('busca').value);"  />
		    </div>
	    </td>
	</tr>
</table>
</form>
</script>
</body>
</html>