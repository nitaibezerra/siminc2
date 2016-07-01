<?php


include_once 'testes/simpleTest/autorun.php';

include_once '../../sase/classes/Mapas.class.inc';
include_once '../../sase/classes/Assessoramento.class.inc';
include_once '../../sase/classes/SituacaoAssessoramento.class.inc';
include_once '../../sase/classes/SituacaoQuestoesPontuaisPar.class.inc';
include_once '../../sase/classes/Orgao.class.inc';
include_once '../../sase/classes/Mesoregiao.class.inc';
include_once '../../sase/classes/Metas.class.inc';
include_once '../../sase/classes/QuestoesPontuaisPar.class.inc';
include_once '../../sase/classes/Territorio.class.inc';


// #########################################################################################################
// código temporario feito para limpar campos duplicados na tabela de sase.assessoramento

global $db;
$sql = "select * from sase.assessoramento ";
$lista = $db->carregar($sql);
// ver($lista,d);

$lista_valida = array();
foreach ($lista as $key => $value) {
	// echo ''.$value['muncod'].'<br/>'; 
	if( array_search($value['muncod'], $lista_valida) !== false ){
		echo 'teste: '.$value['muncod'].'<br/>';
		// ver($lista_valida);
		$sql = "delete from sase.assessoramento where assid = {$value['assid']} ";
		$db->executar( $sql );
		$db->commit();
	}else{
		array_push($lista_valida, $value['muncod']);
	}
	
}

exit;


// #########################################################################################################

class TestOfMapas extends UnitTestCase
{
	public function testMontaEstado(){
		$Mapas = new Mapas();
		
		$Mapas->montaEstado( array('AL','AM'), false ); // brasília, goiânia
		// echo "<pre>";var_dump(json_decode($Mapas->EstadosSelecionados));echo"</pre>";exit;
		// verifica se retorna um json válido
		$this->assertFalse( is_null(json_decode( $Mapas->EstadosSelecionados )) );

		$Mapas->montaEstado( false, array(5300108,5208707) ); // brasília, goiânia
		// echo "<pre>";var_dump(json_decode($Mapas->EstadosSelecionados));echo"</pre>";exit;
		// verifica se retorna um json válido
		$this->assertFalse( is_null(json_decode( $Mapas->EstadosSelecionados )) );

	}

	public function testAplicaCoresAosMunicipios(){
		$muncod = '2700300';
		$Mapas = new Mapas();
		$Mapas->montaEstado( false, array($muncod) ); // Arapica - AL
		$Mapas->aplicaCoresAosMunicipios( 'assessoramento' );

		$sql = " 
			select s.stacor
			from sase.assessoramento a 
			join sase.situacaoassessoramento s on a.stacod = s.stacod 
			where a.muncod = '{$muncod}' ";
		$cor = $Mapas->carregar($sql);

		$decodificado = json_decode($Mapas->EstadosSelecionados);

		foreach ($decodificado as $chave => $valor)
			if( $valor->muncod == $muncod )
				$corBuscada = $valor->cor;

		// verifica se a modificacao da cor alterou corretamente
		$this->assertEqual( $corBuscada, $cor[0]['stacor'] );
	}
}

class TestOfAssessoramento extends UnitTestCase
{
	public function testOfBuscaAssessoramentos(){
		$Assessoramento = new Assessoramento();

		$lista = $Assessoramento->buscaAssessoramentos()->lista;

		// verifica se veio algo na lista
		$this->assertTrue( count($lista) > 0 );

		$lista = $Assessoramento->buscaAssessoramentos( array('estuf'=>'AL') )->lista;
		$diferenteDeFiltro = false;
		foreach ($lista as $chave => $valor)
			if( $valor['estuf'] != 'AL' )
				$diferenteDeFiltro = true;

		// verifica se filtros funcionam
		$this->assertFalse( $diferenteDeFiltro );
	}

	public function testInsereAssessoramento(){
		$Assessoramento = new Assessoramento();
		
		$novoAssessoramento = array(
			'estuf' => 'AL',
			'muncod' => '2700102',
			'stacod' => 0,
			'assstatus' => 'A' );
		$idAssessoramentoInserido = $Assessoramento->insereAssessoramento( $novoAssessoramento )->inserido;

		// verifica se aconteceu de fato a inserção do assessoramento
		$this->assertTrue( $idAssessoramentoInserido != false );
		if( $idAssessoramentoInserido )
			$Assessoramento->excluirAssessoramento( $idAssessoramentoInserido );

		$sql = " select muncod, estuf from sase.assessoramento limit 1 ";
		$assessoramentoBuscado = $Assessoramento->carregar( $sql );
		$novoAssessoramento = array(
			'estuf' => $assessoramentoBuscado[0]['estuf'],
			'muncod' => $assessoramentoBuscado[0]['muncod'],
			'stacod' => 0,
			'assstatus' => 'A' );

		// verifica se a validacao de duplicidade realmente verifica
		$this->assertTrue( $Assessoramento->verificaDuplicidade($novoAssessoramento) == false );

		$novoAssessoramento = array(
			'muncod' => '2700102',
			'stacod' => 0,
			'assstatus' => 'A' );
		$validacaoDadosPorInserir = $Assessoramento->validaDadosPorInserir( $novoAssessoramento );

		// verifica campo estuf
		$this->assertFalse( $validacaoDadosPorInserir );

		$novoAssessoramento = array(
			'estuf' => 'AL',
			'stacod' => 0,
			'assstatus' => 'A' );
		$validacaoDadosPorInserir = $Assessoramento->validaDadosPorInserir( $novoAssessoramento )->inserido;

		// verifica campo muncod
		$this->assertFalse( $validacaoDadosPorInserir );
	}
}


class TestOfSituacaoAssessoramento extends UnitTestCase
{
	public function testExcluirSituacaoAssessoramento(){
		$stacod = 1;

		$SituacaoAssessoramento = new SituacaoAssessoramento();

		$SituacaoAssessoramento->excluirSituacaoAssessoramento( $stacod );
		$this->assertTrue( $SituacaoAssessoramento->stastatus == 'I' );

		$SituacaoAssessoramento->stastatus = 'A';
		$SituacaoAssessoramento->atualizarSituacaoassessoramento();
	}

	public function testAtualizasSituacaoassessoramento(){
		$stacod = 1;

		$SituacaoAssessoramento = new SituacaoAssessoramento();
		$SituacaoAssessoramento->carregarPorId( $stacod );

		$stadscOriginal = $SituacaoAssessoramento->stadsc;
		$SituacaoAssessoramento->stadsc = 'Novo';
		$SituacaoAssessoramento->atualizarSituacaoassessoramento();
		$SituacaoAssessoramento->carregarPorId( $stacod );
		$this->assertEqual($SituacaoAssessoramento->stadsc, 'Novo');

		$SituacaoAssessoramento->stadsc = $stadscOriginal;
		$SituacaoAssessoramento->atualizarSituacaoassessoramento();
	}

	public function testCadastrarSituacaoAssessoramento(){
		$SituacaoAssessoramento = new SituacaoAssessoramento();

		$arDados = array(
			'stacod' => null,
			'stadsc' => 'teste',
			'stacor' => '#000',
			'stastatus' => 'A'
		);

		$SituacaoAssessoramento->popularObjeto( $SituacaoAssessoramento->arCampos, $arDados );
		$SituacaoAssessoramento->cadastrarSituacaoassessoramento();

		$SituacaoAssessoramento->stadsc = '';
		$SituacaoAssessoramento->carregarPorId( $SituacaoAssessoramento->stacod );

		$this->assertEqual( $SituacaoAssessoramento->stadsc, 'teste' );
		$sql = " delete from sase.situacaoassessoramento where stacod = " . $SituacaoAssessoramento->stacod . " ";
		$SituacaoAssessoramento->executar( $sql );
		$SituacaoAssessoramento->commit();
	}
}

class TestOfSituacaoQuestoesPontuaisPar extends UnitTestCase
{
	public function testExcluirSituacaoQuestoesPontuais(){
		$sqpcod = 1;

		$SituacaoQuestoesPontuaisPar = new SituacaoQuestoesPontuaisPar();

		$SituacaoQuestoesPontuaisPar->excluirSituacaoQuestoesPontuais( $sqpcod );
		$this->assertTrue( $SituacaoQuestoesPontuaisPar->sqpstatus == 'I' );

		$SituacaoQuestoesPontuaisPar->sqpstatus = 'A';
		$SituacaoQuestoesPontuaisPar->atualizarSituacaoQuestoesPontuais();
	}

	public function testAtualizarSituacaoQuestoesPontuais(){
		$sqpcod = 1;

		$SituacaoQuestoesPontuaisPar = new SituacaoQuestoesPontuaisPar();
		$SituacaoQuestoesPontuaisPar->carregarPorId( $sqpcod );

		$sqpdscOriginal = $SituacaoQuestoesPontuaisPar->sqpdsc;
		$SituacaoQuestoesPontuaisPar->sqpdsc = 'Novo';
		$SituacaoQuestoesPontuaisPar->atualizarSituacaoQuestoesPontuais();
		$SituacaoQuestoesPontuaisPar->carregarPorId( $sqpcod );
		$this->assertEqual(trim($SituacaoQuestoesPontuaisPar->sqpdsc), 'Novo');

		$SituacaoQuestoesPontuaisPar->sqpdsc = $sqpdscOriginal;
		$SituacaoQuestoesPontuaisPar->atualizarSituacaoQuestoesPontuais();
	}

	public function testCadastrarSituacaoQuestoesPontuais(){
		$SituacaoQuestoesPontuaisPar = new SituacaoQuestoesPontuaisPar();

		$arDados = array(
			'sqpcod' => null,
			'sqpdsc' => 'teste',
			'sqpcor' => '#000',
			'sqpstatus' => 'A'
		);

		$SituacaoQuestoesPontuaisPar->popularObjeto( $SituacaoQuestoesPontuaisPar->arCampos, $arDados );
		$SituacaoQuestoesPontuaisPar->cadastrarSituacaoQuestoesPontuais();

		$SituacaoQuestoesPontuaisPar->sqpdsc = '';
		$SituacaoQuestoesPontuaisPar->carregarPorId( $SituacaoQuestoesPontuaisPar->sqpcod );

		$this->assertEqual( trim($SituacaoQuestoesPontuaisPar->sqpdsc), 'teste' );
		$sql = " delete from sase.situacaoquestoespontuais where sqpcod = " . $SituacaoQuestoesPontuaisPar->sqpcod . " ";
		$SituacaoQuestoesPontuaisPar->executar( $sql );
		$SituacaoQuestoesPontuaisPar->commit();
	}
}

class TestOfOrgao extends UnitTestCase
{
	public function testExcluirOrgao(){
		$orgid = 1;

		$Orgao = new Orgao();

		$Orgao->excluirOrgao( $orgid );
		$this->assertTrue( $Orgao->orgstatus == 'I' );

		$Orgao->orgstatus = 'A';
		$Orgao->atualizarOrgao();
	}

	public function testAtualizarOrgao(){
		$orgid = 1;

		$Orgao = new Orgao();
		$Orgao->carregarPorId( $orgid );

		$orgdscOriginal = $Orgao->orgdsc;
		$Orgao->orgdsc = 'Novo';
		$Orgao->atualizarOrgao();
		$Orgao->carregarPorId( $orgid );
		$this->assertEqual(trim($Orgao->orgdsc), 'Novo');

		$Orgao->orgdsc = $orgdscOriginal;
		$Orgao->atualizarOrgao();
	}

	public function testCadastrarOrgao(){
		$Orgao = new Orgao();

		$maiorId = $Orgao->pegaUm( "select max(orgid) from sase.orgao" );

		$arDados = array(
			'orgid' => null,
			'orgdsc' => 'teste',
			'orgcod' => ($maiorId+1),
			'estuf' => 'GO',
			'orgstatus' => 'A'
		);

		$Orgao->popularObjeto( $Orgao->arCampos, $arDados );
		$Orgao->cadastrarOrgao();

		$Orgao->orgdsc = '';
		$Orgao->carregarPorId( $Orgao->orgid );

		$this->assertEqual( trim($Orgao->orgdsc), 'teste' );
		$sql = " delete from sase.orgao where orgid = " . $Orgao->orgid . " ";
		$Orgao->executar( $sql );
		$Orgao->commit();
	}
}

class TestOfMesoregiao extends UnitTestCase
{
	public function testExcluirMesoregiao(){
		$mesid = 1;

		$Mesoregiao = new Mesoregiao();

		$Mesoregiao->excluirMesoregiao( $mesid );
		$this->assertTrue( $Mesoregiao->messtatus == 'I' );

		$Mesoregiao->messtatus = 'A';
		$Mesoregiao->atualizarMesoregiao();
	}

	public function testAtualizarMesoregiao(){
		$mesid = 1;

		$Mesoregiao = new Mesoregiao();
		$Mesoregiao->carregarPorId( $mesid );

		$mesdscOriginal = $Mesoregiao->mesdsc;
		$Mesoregiao->mesdsc = 'Novo';
		$Mesoregiao->atualizarMesoregiao();
		$Mesoregiao->carregarPorId( $mesid );
		$this->assertEqual(trim($Mesoregiao->mesdsc), 'Novo');

		$Mesoregiao->mesdsc = $mesdscOriginal;
		$Mesoregiao->atualizarMesoregiao();
	}

	public function testCadastrarMesoregiao(){
		$Mesoregiao = new Mesoregiao();

		$maiorId = $Mesoregiao->pegaUm( "select max(mescod) from sase.mesoregiao" );

		$arDados = array(
			'mesdsc' => 'teste',
			'mescod' => ($maiorId+1),
			'orgid' => 1,
			'mescor' => '#000',
			'messtatus' => 'A'
		);

		$Mesoregiao->popularObjeto( $Mesoregiao->arCampos, $arDados );
		$Mesoregiao->cadastrarMesoregiao();

		$Mesoregiao->mesdsc = '';
		$Mesoregiao->carregarPorId( $Mesoregiao->mesid );

		$this->assertEqual( trim($Mesoregiao->mesdsc), 'teste' );
		$sql = " delete from sase.mesoregiao where mesid = " . $Mesoregiao->mesid . " ";
		$Mesoregiao->executar( $sql );
		$Mesoregiao->commit();
	}
}

class TestOfMetas extends UnitTestCase
{
	public function testExcluirMetas(){
		$metid = 1;

		$Metas = new Metas();

		$Metas->excluirMetas( $metid );
		$this->assertTrue( $Metas->metstatus == 'I' );

		$Metas->metstatus = 'A';
		$Metas->atualizarMetas();
	}

	public function testAtualizarMetas(){
		$metid = 1;

		$Metas = new Metas();
		$Metas->carregarPorId( $metid );

		$metdscOriginal = $Metas->mettitulo;
		$Metas->mettitulo = 'Novo';
		$Metas->atualizarMetas();
		$Metas->carregarPorId( $metid );
		$this->assertEqual(trim($Metas->mettitulo), 'Novo');

		$Metas->mettitulo = $metdscOriginal;
		$Metas->atualizarMetas();
	}

	public function testCadastrarMetas(){
		$Metas = new Metas();

		$maiorId = $Metas->pegaUm( "select max(metid) from sase.meta" );

		$arDados = array(
			'mettitulo' => 'teste',
			'metfontemunicipio' => 'teste municipio',
			'metfonteestado' => 'teste estado',
			'metstatus' => 'A'
		);

		$Metas->popularObjeto( $Metas->arCampos, $arDados );
		$Metas->cadastrarMetas();

		$Metas->mettitulo = '';
		$Metas->carregarPorId( $Metas->metid );

		$this->assertEqual( trim($Metas->mettitulo), 'teste' );
		$sql = " delete from sase.meta where metid = " . $Metas->metid . " ";
		$Metas->executar( $sql );
		$Metas->commit();
	}
}

class TestOfQuestoesPontuaisPar extends UnitTestCase
{
	public function testOfBuscaQuestoesPontuaisPar(){
		$QuestoesPontuaisPar = new QuestoesPontuaisPar();

		$lista = $QuestoesPontuaisPar->buscaQuestoesPontuaisPar()->lista;

		// verifica se veio algo na lista
		$this->assertTrue( count($lista) > 0 );

		$lista = $QuestoesPontuaisPar->buscaQuestoesPontuaisPar( array('estuf'=>'RR') )->lista;
		$diferenteDeFiltro = false;
		foreach ($lista as $chave => $valor)
			if( $valor['estuf'] != 'RR' )
				$diferenteDeFiltro = true;

		// verifica se filtros funcionam
		$this->assertFalse( $diferenteDeFiltro );
	}

	public function testInsereQuestoesPontuaisPar(){
		$QuestoesPontuaisPar = new QuestoesPontuaisPar();
		
		$novoQuestoesPontuaisPar = array(
			'estuf' => 'AL',
			'muncod' => '2700102',
			'sqpcod' => 0,
			'qptstatus' => 'A' );
		$idQuestoesPontuaisParInserido = $QuestoesPontuaisPar->insereQuestoesPontuaisPar( $novoQuestoesPontuaisPar )->inserido;

		// verifica se aconteceu de fato a inserção do questoespontuais
		$this->assertTrue( $idQuestoesPontuaisParInserido != false );
		if( $idQuestoesPontuaisParInserido ){
			$sql = " delete from sase.questoespontuaispar where qptid = {$idQuestoesPontuaisParInserido} ";
			$QuestoesPontuaisPar->executar( $sql );
			$QuestoesPontuaisPar->commit();
		}

		$sql = " select muncod, estuf from sase.questoespontuaispar limit 1 ";
		$questoespontuaisBuscado = $QuestoesPontuaisPar->carregar( $sql );
		$novoQuestoesPontuaisPar = array(
			'estuf' => $questoespontuaisBuscado[0]['estuf'],
			'muncod' => $questoespontuaisBuscado[0]['muncod'],
			'sqpcod' => 0,
			'qptstatus' => 'A' );

		// verifica se a validacao de duplicidade realmente verifica
		$this->assertTrue( $QuestoesPontuaisPar->verificaDuplicidade($novoQuestoesPontuaisPar) == false );

		$novoQuestoesPontuaisPar = array(
			'muncod' => '2700102',
			'sqpcod' => 0,
			'qptstatus' => 'A' );
		$validacaoDadosPorInserir = $QuestoesPontuaisPar->validaDadosPorInserir( $novoQuestoesPontuaisPar );

		// verifica campo estuf
		$this->assertFalse( $validacaoDadosPorInserir );

		$novoQuestoesPontuaisPar = array(
			'estuf' => 'AL',
			'sqpcod' => 0,
			'qptstatus' => 'A' );
		$validacaoDadosPorInserir = $QuestoesPontuaisPar->validaDadosPorInserir( $novoQuestoesPontuaisPar )->inserido;

		// verifica campo muncod
		$this->assertFalse( $validacaoDadosPorInserir );
	}
}

class TestOfTerritorio extends UnitTestCase
{
	public function testExcluirTerritorio(){
		$terid = 659;

		$Territorio = new Territorio();

		$Territorio->excluirTerritorio( $terid );
		$this->assertTrue( $Territorio->terstatus == 'I' );

		$Territorio->terstatus = 'A';
		$Territorio->atualizarTerritorio();
	}

	public function testAtualizarTerritorio(){
		$terid = 1;

		$Territorio = new Territorio();
		$Territorio->carregarPorId( $terid );

		$usucpfOriginal = $Territorio->usucpf;
		$Territorio->usucpf = 'Novo';
		$Territorio->atualizarTerritorio();
		$Territorio->carregarPorId( $terid );
		$this->assertEqual(trim($Territorio->usucpf), 'Novo');

		$Territorio->usucpf = $usucpfOriginal;
		$Territorio->atualizarTerritorio();
	}

	public function testCadastrarTerritorio(){
		$Territorio = new Territorio();

		$maiorId = $Territorio->pegaUm( "select max(terid) from sase.territorio" );

		$arDados = array(
			'mesid' => '1',
			'muncod' => '5300108',
			'terstatus' => 'A',
			'usucpf' => ''
		);

		$Territorio->popularObjeto( $Territorio->arCampos, $arDados );
		$Territorio->cadastrarTerritorio();

		$Territorio->usucpf = '';
		$Territorio->carregarPorId( $Territorio->terid );

		$this->assertEqual( trim($Territorio->usucpf), '' );
		$sql = " delete from sase.territorio where terid = " . $Territorio->terid . " ";
		$Territorio->executar( $sql );
		$Territorio->commit();
	}
}