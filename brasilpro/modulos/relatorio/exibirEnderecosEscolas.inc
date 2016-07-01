<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>

<?php
	//ver( $_SESSION["arEscolas"] );
	if( !isset( $_SESSION["arEscolas"] ) ){ die(); }

	if(is_array($_SESSION["arEscolas"])){
		foreach($_SESSION["arEscolas"] as $idEscola){
			if($idEscola){
				$arrEscolas[] = $idEscola;
			} 
		}
	}
	
	$sql = "select e.entcodent, e.entnome, m.estuf, m.mundescricao, d.endbai, 
			( coalesce(d.endlog,'') || ' ' || coalesce(d.endcom,'') || ' ' || coalesce(d.endnum,'')) as endereco,  
			'<script>document.write( mascaraglobal( \'#####-###\', ' || d.endcep || ' ) )</script>' as cep, m.muncod,
			e.entemail as email,
			'(' || e.entnumdddcomercial || ') ' || e.entnumcomercial as telefone
			from entidade.entidade e
				inner join entidade.endereco d on e.entid = d.entid
				inner join entidade.funcaoentidade fe ON fe.entid = e.entid 
				left join territorios.municipio m on m.muncod = d.muncod 
			where e.entid in (". implode( ", ", $arrEscolas ).")
			and fe.funid = 3 
			and e.tpcid = 1
			order by entnome, m.estuf, m.mundescricao";
	
	$arCabecalho = array( 'Código INEP','Escola', 'UF', 'Município', 'Bairro', 'Endereço',  'CEP', 'Código IBGE', 'Email', 'Telefone' );
	$db->monta_lista( $sql, $arCabecalho, 1000, 1000, 'N', '100%','N', true );
?>