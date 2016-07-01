<?php 

include_once APPRAIZ . 'includes/classes/Modelo.class.inc';

class Cockpit_Mais_Medicos extends Modelo {
	
	public function contaMedicosPorRegiao()
	{
		$sql = "select
					regdescricao as descricao,
					count(distinct mdc.mdccpf) as valor
				from maismedicos.medico mdc
				join territorios.municipio mun on mun.muncod = mdc.muncod
				join territorios.estado est on est.estuf = mun.estuf
				join territorios.regiao reg on reg.regcod = est.regcod
				where mdc.mdcstatus = 'A'
				group by regdescricao
				order by regdescricao";
	
		return $this->carregar($sql);
	}
	
	public function contaIndicadorPorRegiao($indid=null)
	{
		if($indid){
			$sql = "SELECT 
							regdescricao as descricao,
							sum(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN territorios.estado est ON dsh.dshuf = est.estuf
						INNER JOIN territorios.regiao reg ON reg.regcod = est.regcod
						WHERE sh.indid IN ({$indid})
						AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
						AND sh.sehstatus <> 'I'
						GROUP BY regdescricao
						ORDER BY regdescricao";
			
			return $this->carregar($sql);
		}
		return false;
	}
	public function contaTutoresSupervisoresPorRegiaoAntigo($tipo='S')
	{
		$sql = "select
					regdescricao as descricao,
					count(distinct tut.tutcpf) as valor
				from maismedicos.tutor tut
				join territorios.municipio mun on mun.muncod = tut.muncod
				join territorios.estado est on est.estuf = mun.estuf
				join territorios.regiao reg on reg.regcod = est.regcod
				where tut.tutstatus = 'A'
				and tutvalidade = true
				and tut.tuttipo = '{$tipo}'
				group by regdescricao
				order by regdescricao";
	
		return $this->carregar($sql);
	}
	
	public function contaTutoresPorRegiao($tipo='S')
	{
		return $this->contaTutoresSupervisoresPorRegiao('T');
	}
	
	public function contaSupervisoresPorRegiao($tipo='S')
	{
		return $this->contaTutoresSupervisoresPorRegiao('S');
	}
	
	public function contaMunicipiosPorRegiao()
	{
		$sql = "select
					regdescricao as descricao,
					count(distinct mun.muncod) as valor
				from territorios.municipio mun 
				join territorios.estado est on est.estuf = mun.estuf
				join territorios.regiao reg on reg.regcod = est.regcod
				group by regdescricao
				order by regdescricao";
	
		return $this->carregar($sql);
	}

	public function montaGraficoBarrasPorRegiao($metodo = 'contaMedicosPorRegiao')
	{
		$rsPro = $this->$metodo();
		foreach($rsPro as $pro){
			$arrRegiao[] = $pro['descricao'];
			$arrProfisionalPorRegiao[] = $pro['valor'];
		}
		
		switch ($metodo){
			case 'contaMunicipiosPorRegiao':
				$titulo = 'Municípios';
				$tituloEixoY = 'Qtde. de municípios';
				$label = "Qtde. de municípios: ";
				break;
			case 'contaSupervisoresPorRegiao':
				$titulo = 'Supervisores';
				$tituloEixoY = 'Qtde. de profissionais ativos';
				$label = "Qtde. de supervisores: ";
				break;
			case 'contaTutoresPorRegiao':
				$titulo = 'Tutores';
				$tituloEixoY = 'Qtde. de profissionais ativos';
				$label = "Qtde. de tutores: ";
				break;
			case 'contaMedicosPorRegiao':
				$titulo = 'Médicos';
				$tituloEixoY = 'Qtde. de profissionais ativos';
				$label = "Qtde. de médicos: ";
				break;
			default:
				$titulo = '-';
				$tituloEixoY = "-";
				$label = "-";
				break;
		}
		?>
			<script type="text/javascript">
			
				jQuery(function () {
					
			        jQuery('#graficoProfiPorRegiao').highcharts({
			            chart: {
			                type: 'column',
			                /*margin: [ 50, 50, 100, 80],*/
			                height: 350
			            },
			            title: {
			                text: '<?php echo $titulo; ?>'
			            },
			            xAxis: {
			                categories: [<?php echo "'".implode("','", $arrRegiao)."'"; ?>],
			                labels: {
			                    rotation: -45,
			                    align: 'right',
			                    style: {
			                        fontSize: '13px',
			                        fontFamily: 'Verdana, sans-serif'
			                    }
			                }
			            },
			            yAxis: {
			                min: 0,
			                title: {
			                    text: '<?php echo $tituloEixoY; ?>'
			                }
			            },
			            legend: {
			                enabled: false
			            },
			            tooltip: {
			                formatter: function() {
			                    return '<b>'+ this.x +'</b><br/>'+
			                        '<?php echo $label; ?> '+ Highcharts.numberFormat(this.y, 1) +
			                        '';
			                }
			            },
			            series: [{
			                name: 'Population',
			                data: [<?php echo implode(',', $arrProfisionalPorRegiao); ?>],
			                dataLabels: {
			                    enabled: true,
			                    rotation: -90,
			                    color: '#FFFFFF',
			                    align: 'right',
			                    x: 4,
			                    y: 10,
			                    style: {
			                        fontSize: '13px',
			                        fontFamily: 'Verdana, sans-serif'
			                    }
			                }
			            }]
			        });
			    });
				    
			</script>
			<div id="graficoProfiPorRegiao"></div>
		<?php
	}
	
	
	
	public function montaGraficoPeriodoSemVisitaMedico($post = null)
	{
		if($post) extract($post);
		
		if($dsei){
			$arWhere[] = "mdc.dsei = ".($dsei=='S'?'true':'false');
		}

		if($estuf){
			$arWhere[] = "mdc.estuf = '{$estuf}'";
		}
		if($muncod){
			$arWhere[] = "mdc.muncod = '{$muncod}'";
		}
		if($regcod){
			$arWhere[] = "reg.regcod = '{$regcod}'";
		}
		if($ciclo){
			$arWhere[] = "mdc.nuciclo ".($ciclo=='sem' ? 'is null' : "= '{$ciclo}'");
		}
		if($uniid){
			$arWhere[] = "mdc.uniid = {$uniid}";
		}
		
		$sql = "select
					count(distinct mdccpf) as medicos,
					uniid,uninome, unisigla,
					case when dtvisita = 'semvisita' then 'semvisita'
						when dtvisita = 'semdata' then 'semdata'
						when to_char(now() - to_date(dtvisita, 'YYYY-MM-DD'), 'DD')::integer <= 30 then 'menos30'
						when to_char(now() - to_date(dtvisita, 'YYYY-MM-DD'), 'DD')::integer >= 31 and to_char(now() - to_date(dtvisita, 'YYYY-MM-DD'), 'DD')::integer <= 60 then 'entre30e60'
						when to_char(now() - to_date(dtvisita, 'YYYY-MM-DD'), 'DD')::integer >= 61 and to_char(now() - to_date(dtvisita, 'YYYY-MM-DD'), 'DD')::integer <= 90 then 'entre60e90'
						when to_char(now() - to_date(dtvisita, 'YYYY-MM-DD'), 'DD')::integer > 90 then 'mais90'
					end as periodo
				from (
					select
						mdc.mdccpf,
						uni.uniid,
						uni.uninome,
						uni.unisigla || ' - ' || uni.estuf as unisigla,
						uni.estuf,
						case when cpfprofissional is null then 'semvisita'
							when max(fom.dtvisita) is null and cpfprofissional is null then 'semdata'
							else to_char(max(fom.dtvisita), 'YYYY-MM-DD') end as dtvisita
					from maismedicos.medico mdc
					left join maismedicos.ws_respostas_formulario fom  on mdc.mdccpf = fom.cpfprofissional
						and fom.wssstatus = 'A'
						and fom.status = 'Finalizado'
						and fom.idformulario in (35,36,40,43)
					join maismedicos.universidade uni on uni.uniid = mdc.uniid
						and uni.unistatus = 'A'
					join territorios.estado est on mdc.estuf = est.estuf 
					join territorios.regiao reg on reg.regcod = est.regcod
					where mdc.mdcstatus = 'A'
					".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')."
					group by mdc.mdccpf,uni.uniid,uni.uninome,uni.unisigla,cpfprofissional,uni.estuf
					
				) as foo
	
				group by uniid, uninome,unisigla, periodo, estuf
				order by estuf, uninome";
		
		$rsPeriodosSemVisita = $this->carregar($sql);
		
		$arPeriodos = array(
				'menos30'=>'Menos de 30 dias',
				'entre30e60'=>'Entre 30 e 60 dias',
				'entre60e90'=>'Entre 60 e 90 dias',
				'mais90'=>'Mais de 90 dias',
				'semvisita'=>'Sem visita'
		);
		
		if($rsPeriodosSemVisita){
			foreach($rsPeriodosSemVisita as $dados){
				$arrCategories[$dados['uniid']] = $dados['unisigla'];
				$arrSeries[$dados['uniid']][$dados['periodo']] += $dados['medicos'];
			}
		}
		
		foreach($arPeriodos as $k=>$p)
		{
			foreach($arrCategories as $i=>$v){
				$valor = $arrSeries[$i][$k];
				$arValorPorPeriodo[$i] = $valor ? $valor : 0;
			}
			$arSeriePorPeriodo[] = "{name: '{$p}', data: [".implode(', ', $arValorPorPeriodo)."]}";
		}
		?>
		<script>
				jQuery(function(){
	                	
                	jQuery('#graficoPeriodoSemVisita').highcharts({
	                        chart: {
	                            type: 'bar',
	                            alignTicks: true,
	                            animation: true,
	                            height: <?php echo (count($arrCategories)*15)+200 ?>
	                            
	                        },
	                    xAxis: {
	                            categories: [<?php echo "'".implode("','", $arrCategories)."'"; ?>],
		                            labels: {
		                                rotation: 0,
		                                align: 'right',
		                                useHTML: true,                
		                                style: {
		                                    fontSize: '9px',
		                                    fontFamily: 'proxima-nova,helvetica,arial,sans-seri',
		                                    whiteSpace: 'nowrap',
	                                	}
                            		}
                            		
                        	},
	                        yAxis: {
	                            min: 0,
	                            title: {text: 'Médicos'}
	                        },
	                        legend: {
	                            backgroundColor: '#FFFFFF',
	                            reversed: true,
	                            verticalAlign: 'top'
	                        },
	                        plotOptions: {
	                            series: {
	                                stacking: 'normal'
	                            }
	                        },
                            series: [<?php echo implode(",", $arSeriePorPeriodo); ?>]
		                    });
	                	});
                	
                	</script>
                	<div id="graficoPeriodoSemVisita"></div>
	<?php 
	}
	
}