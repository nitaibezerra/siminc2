<?php


/**
 * Polígonos
 */
Interface PoligonosInterface{
	public function getMunicipios();
	public function getPais();
}
class Poligonos extends Modelo implements PoligonosInterface
{
	public $estuf = array();
	public $muncod = array();
	public $orgid;

	public $poligono;

	/**
	 * Busca Municípios e devolve json
	 *
	 * @param $origem
	 * @return json
	 */
	public function getMunicipios(){
		$precisao = $this->trataPrecisao( $this->estuf );

		$sql = "
			SELECT DISTINCT 
				ST_asGeoJSON(ST_transform(ST_simplify(ST_transform(munpoligono, 2249), {$precisao}),4291),2, 0) as poli,
				mun.muncod, 
				removeacento(mundescricao) as mundescricao, 
				mun.estuf, 
				'#f6ead9' as cor 
			from 
				territoriosgeo.municipio mun 
			" . ( !empty($this->orgid) ? " 
				join sase.territorio t on t.muncod = mun.muncod 
				join sase.mesoregiao m on m.mesid = t.mesid
				join sase.orgaoestado oes on oes.oesid = m.oesid
				join sase.orgao o on oes.orgid = o.orgid and o.orgid = '" . $this->orgid . "'
			":"" ) . " 
			where 
				munpoligono is not null 
				".( !empty($this->estuf) ? " and mun.estuf in ('".(implode("','",$this->estuf))."')" : " " )." 
				".( !empty($this->muncod) ? " and mun.muncod in ('".(implode("','",$this->muncod))."') " : " " );

		$dados = $this->carregar($sql,3600);

		$this->poligono = $this->trataGeoJson($dados);
	}

	/**
	 * Busca Estado e devolve json
	 *
	 * @param $origem
	 * @return json
	 */
	public function getEstado(){
		$precisao = $this->trataPrecisao( $this->estuf );

		$sql = "
			SELECT DISTINCT 
				ST_asGeoJSON(ST_transform(ST_simplify(ST_transform(estpoligono, 2249), {$precisao}),4291),2, 0) as poli,
				est.estuf, 
				removeacento(estdescricao) as estdescricao, 
				est.estuf, 
				'#f6ead9' as cor 
			from 
				territoriosgeo.estado est
			where 
				estpoligono is not null 
				".( !empty($this->estuf) ? " and est.estuf in ('".(implode("','",$this->estuf))."')" : " " );

		$dados = $this->carregar($sql,3600);

		$this->poligono = $this->trataGeoJson($dados);
	}

	/**
	 * Monta País
	 *
	 * @internal atualmente só o Brasil
	 * @return json
	 */
	public function getPais(){
		
		$sql = "
			SELECT DISTINCT 
				ST_asGeoJSON(ST_transform(ST_simplify(ST_transform(estpoligono, 2249), 6000),4291),2, 0) as poli,
				est.estuf, 
				removeacento(estdescricao) as estdescricao, 
				'#f3bf47' as cor 
			from 
				territoriosgeo.estado est 
			-- where estuf = 'GO' 
		";
		$dados = $this->carregar($sql,3600);

		$this->poligono = $this->trataGeoJson($dados);
		// echo "<pre>";print_r($this->poligono);exit;
	}

	/**
	 * Função para tratar precisao do poligono gerado pelo PostGis - existem estados 
	 * que, por serem pequenos, podem ter maior precisão.
	 *
	 * @param array $estuf
	 * @return integer based on Ramer–Douglas–Peucker algorithm
	 * @todo entender o algoritmo e aplicar aqui =p
	 */
	public function trataPrecisao( $estuf ){
		$valorDefault = 6000;

		if( count($estuf) > 1 ) return $valorDefault;

		switch ($estuf[0]) {
			case 'AL':
			case 'AC':
			case 'AP':
			case 'PB':
			case 'PE':
			case 'RJ':
			case 'RN':
			case 'RO':
			case 'RR':
			case 'SE':
				$valorDefault = 1500;
				break;
		}

		return $valorDefault;
	}

	/**
	 * Tratamento do resultado geojson da query
	 *
	 * @param json $geojson
	 */	
	private function trataGeoJson( $geojson ){
		$geojson = simec_json_encode( $geojson);
        //$geojson = json_encode( $geojson);
		$geojson = str_replace('"{','{',$geojson);
		$geojson = str_replace('}"','}',$geojson);
		$geojson = str_replace('\\','',$geojson);
		return $geojson;
	}

	/**
	 * FUNCAO FEITA PARA AJUSTAR OS POLIGONOS DO BANCO DE DADOS DE ESTADOS
	 */
	public function ajusteJSONEstados(){
		return;

		$sql1 = " SELECT estuf, ST_asGeoJSON(ST_transform(estpoligono,4291),2, 0) as poli
			from territoriosgeo.estado
			where estuf in ('MA','RJ','PE','SC','PA') ";
		$estados = $this->carregar( $sql1 );
		
		$utilitario = 0;
		foreach ($estados as $key => $value) {
			
			$estados[$key]['poli'] = json_decode($value['poli']);
		
			$contador = 0;
			if( count($estados[$key]['poli']->coordinates) > 1 )
			foreach ($estados[$key]['poli']->coordinates as $key2 => $value2) {

				if( $utilitario < count($estados[$key]['poli']->coordinates[$key2][0]) ){
					$estados[$key]['poli']->coordinates[0] = $estados[$key]['poli']->coordinates[$key2];
					$utilitario = count($estados[$key]['poli']->coordinates[$key2][0]);
				}

			}
			
			// CONVERSAO PARA LINESTRING NOVAMENTE
				$linear = "MULTIPOLYGON(((";
				$linearData = array();
				if( !empty($estados[$key]['poli']->coordinates[0][0]) ){
					foreach ($estados[$key]['poli']->coordinates[0][0] as $key2 => $value2) {
						array_push($linearData, $value2[0]." ".$value2[1]);
					}
				}else{ continue; }
				$linearTemp = implode(',', $linearData);
				$linear .= $linearTemp.")))";
				echo "teste: ".$linear;
				$novoPoligono = "ST_GeomFromText('".$linear."',4291)";
			// --
			
			$sql = " select ".$novoPoligono." as teste ";
			$linear = $this->pegaUm( $sql );
		
			$sql = " update territoriosgeo.estado set estpoligono = '".$linear."' where estuf = '".$value['estuf']."' ";
			$resultado = $this->executar( $sql );
			$this->commit();
		
		}
	
	}

	/**
	 * FUNCAO FEITA PARA AJUSTAR OS POLIGONOS DO BANCO DE DADOS DE MUNICIPIOS
	 */
	public function ajusteJSONMunicipios(){
		return;
		
		$sql1 = " SELECT muncod, ST_asGeoJSON(ST_transform(munpoligono,4291),2, 0) as poli
			from territoriosgeo.municipio ";
		$municipios = $this->carregar( $sql1 );
		// echo "<pre>";print_r($municipios);exit;
		
		$utilitario = 0;
		foreach ($municipios as $key => $value) {
			
			$municipios[$key]['poli'] = json_decode($value['poli']);
			// echo "<pre>";print_r($municipios);exit;
		
			$contador = 0;
			// echo "<pre>";print_r($municipios[$key]['poli']->coordinates);exit;
			if( count($municipios[$key]['poli']->coordinates) > 1 )
			foreach ($municipios[$key]['poli']->coordinates as $key2 => $value2) {

				if( $utilitario < count($municipios[$key]['poli']->coordinates[$key2][0]) ){
					$municipios[$key]['poli']->coordinates[0] = $municipios[$key]['poli']->coordinates[$key2];
					$utilitario = count($municipios[$key]['poli']->coordinates[$key2][0]);
				}

			}
			// echo "<pre>";print_r($municipios[$key]['poli']->coordinates);exit;
			
			// CONVERSAO PARA LINESTRING NOVAMENTE
				$linear = "MULTIPOLYGON(((";
				$linearData = array();
				if( !empty($municipios[$key]['poli']->coordinates[0][0]) ){
					foreach ($municipios[$key]['poli']->coordinates[0][0] as $key2 => $value2) {
						array_push($linearData, $value2[0]." ".$value2[1]);
					}
				}else{ continue; }
				$linearTemp = implode(',', $linearData);
				$linear .= $linearTemp.")))";
				// echo "teste: ".$linear;
				$novoPoligono = "ST_GeomFromText('".$linear."',4291)";
			// --
			
			$sql = " select ".$novoPoligono." as teste ";
			$linear = $this->pegaUm( $sql );
		
			$sql = " update territoriosgeo.estado set estpoligono = '".$linear."' where muncod = '".$value['muncod']."' ";
			$resultado = $this->executar( $sql );
			$this->commit();
		
		}
	
	}
}